module;
#include "dx.h"
module RootSignature;

import RenderSystem;
import RootSignatureParams;
import Shader;

D3D12_STATIC_SAMPLER_DESC getSamplerDesc(int32 shaderRegister, D3D12_FILTER filter)
{
    D3D12_STATIC_SAMPLER_DESC sampler = {};
    sampler.Filter = filter;
    sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    sampler.MipLODBias = 0;
    sampler.MaxAnisotropy = 0;
    sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
    sampler.MinLOD = 0.0f;
    sampler.MaxLOD = D3D12_FLOAT32_MAX;
    sampler.ShaderRegister = shaderRegister;
    sampler.RegisterSpace = 0;
    sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    return sampler;
}

void RootSignature::init(RenderSystem& renderSystem, Array<CD3DX12_ROOT_PARAMETER>& parameters)
{
    auto defaultSampler = getSamplerDesc(0, D3D12_FILTER_ANISOTROPIC);
    auto pointSampler = getSamplerDesc(1, D3D12_FILTER_MIN_MAG_MIP_POINT);
    auto linearSampler = getSamplerDesc(2, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

    D3D12_STATIC_SAMPLER_DESC staticSamplers[3] = {defaultSampler, pointSampler, linearSampler};

    CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
    auto Flags = D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED | D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    rootSignatureDesc.Init(parameters.count(), &parameters[0], 3, staticSamplers, Flags);   

    MyPtr<ID3DBlob> signature;
    MyPtr<ID3DBlob> error;
    auto result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    if (FAILED(result))
        throw;

    auto device = renderSystem.get_device();
    result = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&signatureImpl));
    if (FAILED(result))
        throw;
}

MainRootSignature::MainRootSignature(RenderSystem& renderSystem)
{
    Array<CD3DX12_ROOT_PARAMETER> parameters;
    parameters.resize(Params::Count);

    parameters[Params::MaterialsConstantBufferView].InitAsConstantBufferView(0); // b0

    parameters[Params::FrameConstantBufferView].InitAsConstantBufferView(1);   // b1;
    parameters[Params::GBufferConstantBufferView].InitAsConstantBufferView(2); // b2;

    parameters[Params::MeshConstantBufferView].InitAsConstantBufferView(3); // b3
    parameters[Params::MaterialInlineConstants].InitAsConstants(1, 4);      // b4

    init(renderSystem, parameters);
}

ComputeRootSignature::ComputeRootSignature(RenderSystem& renderSystem)
{
    Array<CD3DX12_ROOT_PARAMETER> parameters;
    parameters.resize(Params::Count);

    parameters[Params::ConstantBufferView].InitAsConstantBufferView(0);                                                                 // b0
    parameters[Params::ShaderResourceView].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0));  // t0
    parameters[Params::UnorderedAccessView].InitAsDescriptorTable(1, &CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_UAV, 1, 0)); // u0

    init(renderSystem, parameters);
}
