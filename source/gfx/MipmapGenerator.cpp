module;
#include "assert.h"
#include "dx.h"
module mipmap_generator;

import render_system;
import assets;
import shader;
import shader_manager;
import descriptor_handle;
import oneshot_allocator;

struct ConstantData
{
    Vector2 InvOutTexelSize;
    uint32 SrcMipIndex;
};

MipMapGenerator::MipMapGenerator()
{
    auto& renderSystem = Single::Get<RenderSystem>();

    auto shader = ShaderManager::GetInstance().getComputeShader("shaders/mipmaps.hlsl", "CSMain");

    D3D12_COMPUTE_PIPELINE_STATE_DESC desc = {};
    desc.CS = CD3DX12_SHADER_BYTECODE(shader->bytecode.Get());
    desc.pRootSignature = renderSystem.getComputeRootSignature().signatureImpl.Get();

    renderSystem.get_device()->CreateComputePipelineState(&desc, IID_PPV_ARGS(&pso));
}

void MipMapGenerator::Generate(Texture& texture, CommandList& commandList)
{
    const auto desc = texture.getInternal()->GetDesc();

    auto stagingDesc = desc;
    stagingDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    stagingDesc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;

    auto& renderSystem = Single::Get<RenderSystem>();
    auto device = renderSystem.get_device();

    const CD3DX12_HEAP_PROPERTIES defaultHeapProperties(D3D12_HEAP_TYPE_DEFAULT);

    GpuResource& staging = *new GpuResource(); // leak
    staging.reinit(stagingDesc, D3D12_RESOURCE_STATE_COPY_DEST);

    commandList.transition(texture, D3D12_RESOURCE_STATE_COPY_SOURCE);

    const CD3DX12_TEXTURE_COPY_LOCATION src(texture.getInternal(), 0);
    const CD3DX12_TEXTURE_COPY_LOCATION dst(staging.getInternal(), 0);
    commandList.listImpl->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);

    commandList.transition(staging, D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);

    MyPtr<ID3D12DescriptorHeap> descriptorHeap;
    D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
    descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    descriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    descriptorHeapDesc.NumDescriptors = desc.MipLevels;
    device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
    descriptorHeap.Get()->AddRef();

    auto const descriptorSize = static_cast<int>(device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

    CD3DX12_CPU_DESCRIPTOR_HANDLE handleIt(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

    D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = desc.Format;
    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srvDesc.Texture2D.MostDetailedMip = 0;
    srvDesc.Texture2D.MipLevels = desc.MipLevels;

    device->CreateShaderResourceView(staging.getInternal(), &srvDesc, handleIt);

    // Create the UAVs for the tail
    for (uint16 mip = 1; mip < desc.MipLevels; ++mip)
    {
        D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
        uavDesc.Format = desc.Format;
        uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
        uavDesc.Texture2D.MipSlice = mip;

        handleIt.Offset(descriptorSize);
        device->CreateUnorderedAccessView(staging.getInternal(), nullptr, &uavDesc, handleIt);
    }

    // Set up UAV barrier (used in loop)
    D3D12_RESOURCE_BARRIER barrierUAV = {};
    barrierUAV.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
    barrierUAV.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrierUAV.UAV.pResource = staging.getInternal();

    // Barrier for transitioning the subresources to UAVs
    D3D12_RESOURCE_BARRIER srv2uavDesc = {};
    srv2uavDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    srv2uavDesc.Transition.pResource = staging.getInternal();
    srv2uavDesc.Transition.Subresource = 0;
    srv2uavDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
    srv2uavDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;

    // Barrier for transitioning the subresources to SRVs
    D3D12_RESOURCE_BARRIER uav2srvDesc = {};
    uav2srvDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    uav2srvDesc.Transition.pResource = staging.getInternal();
    uav2srvDesc.Transition.Subresource = 0;
    uav2srvDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    uav2srvDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;

    auto& computeContext = commandList.getComputeContext();

    // Set up state
    computeContext.setupRoot();
    commandList.listImpl->SetPipelineState(pso.Get());
    commandList.listImpl->SetDescriptorHeaps(1, descriptorHeap.GetAddressOf());

    D3D12_GPU_DESCRIPTOR_HANDLE handle(descriptorHeap->GetGPUDescriptorHandleForHeapStart());
    computeContext.setTexture(handle);

    // Get the descriptor handle -- uavH will increment over each loop
    CD3DX12_GPU_DESCRIPTOR_HANDLE uavH(handle, descriptorSize); // offset by 1 descriptor

    // Process each mip
    auto mipWidth = static_cast<uint32>(desc.Width);
    uint32 mipHeight = desc.Height;
    for (uint32 mip = 1; mip < desc.MipLevels; ++mip)
    {
        mipWidth = std::max<uint32>(1, mipWidth >> 1);
        mipHeight = std::max<uint32>(1, mipHeight >> 1);

        // Transition the mip to a UAV
        srv2uavDesc.Transition.Subresource = mip;
        commandList.listImpl->ResourceBarrier(1, &srv2uavDesc);

        // Bind the mip subresources
        computeContext.setUnorderedAccessView(uavH);

        // Set constants
        auto constants = renderSystem.getOneshotAllocator().Allocate<ConstantData>();
        constants->SrcMipIndex = mip - 1;
        constants->InvOutTexelSize = Vector2(1.0f / float(mipWidth), 1.0f / float(mipHeight));
        computeContext.setConstants(constants.GpuAddress);

        constexpr int ThreadGroupSize = 8;

        // Process this mip
        commandList.listImpl->Dispatch(
            (mipWidth + ThreadGroupSize - 1) / ThreadGroupSize,
            (mipHeight + ThreadGroupSize - 1) / ThreadGroupSize,
            1);

        commandList.listImpl->ResourceBarrier(1, &barrierUAV);

        // Transition the mip to an SRV
        uav2srvDesc.Transition.Subresource = mip;
        commandList.listImpl->ResourceBarrier(1, &uav2srvDesc);

        // Offset the descriptor heap handles
        uavH.Offset(descriptorSize);
    }

    commandList.transition(staging, D3D12_RESOURCE_STATE_COPY_SOURCE);
    commandList.transition(texture, D3D12_RESOURCE_STATE_COPY_DEST);

    //// Copy the entire resource back
    commandList.listImpl->CopyResource(texture.getInternal(), staging.getInternal());

    //// Transition the target resource back to pixel shader resource
    commandList.transition(texture, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}
