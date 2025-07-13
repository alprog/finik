module compute_context;

import render_system;
import root_signature;

using Params = ComputeRootSignature::Params;

ComputeContext::ComputeContext(RenderSystem& renderSystem, ID3D12GraphicsCommandList& commandList)
    : renderSystem{renderSystem}
    , commandList{commandList}
{
}

void ComputeContext::setupRoot()
{
    commandList.SetComputeRootSignature(renderSystem.getComputeRootSignature().signatureImpl.Get());
}

void ComputeContext::setConstants(D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
{
    commandList.SetComputeRootConstantBufferView(Params::ConstantBufferView, gpuAddress);
}

void ComputeContext::setTexture(D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
    commandList.SetComputeRootDescriptorTable(Params::ShaderResourceView, handle);
}

void ComputeContext::setUnorderedAccessView(D3D12_GPU_DESCRIPTOR_HANDLE handle)
{
    commandList.SetComputeRootDescriptorTable(Params::UnorderedAccessView, handle);
}
