module ComputeContext;

import RenderSystem;
import RootSignature;

using Params = ComputeRootSignature::Params;

ComputeContext::ComputeContext(RenderEngine& engine, ID3D12GraphicsCommandList& commandList)
    : engine{engine}
    , commandList{commandList}
{
}

void ComputeContext::setupRoot()
{
    commandList.SetComputeRootSignature(engine.getComputeRootSignature().signatureImpl.Get());
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
