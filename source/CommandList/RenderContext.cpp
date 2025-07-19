module RenderContext;

import ConstantBuffer;
import RenderSystem;
import RootSignatureParams;
import RootSignature;
import Mesh;
import MaterialManager;
import UploadBuffer;

using Params = MainRootSignature::Params;

RenderContext::RenderContext(RenderSystem& renderSystem, ID3D12GraphicsCommandList& commandList)
    : renderSystem{renderSystem}
    , commandList{commandList}
{
}

void RenderContext::setupRoot()
{
    commandList.SetGraphicsRootSignature(renderSystem.getRootSignature().signatureImpl.Get());

    UploadBuffer* uploadBuffer = MaterialManager::GetInstance().ConstantBuffer->uploadBuffer;
    auto address = uploadBuffer->GetGPUVirtualAddress();
    commandList.SetGraphicsRootConstantBufferView(Params::MaterialsConstantBufferView, address);
}

void RenderContext::setFrameConstants(D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
{
    commandList.SetGraphicsRootConstantBufferView(Params::FrameConstantBufferView, gpuAddress);
}

void RenderContext::setGBufferConstants(D3D12_GPU_VIRTUAL_ADDRESS gpuAddress)
{
    commandList.SetGraphicsRootConstantBufferView(Params::GBufferConstantBufferView, gpuAddress);
}

void RenderContext::setModelMatrix(const Matrix& matrix)
{
    setModelMatrix(matrix, matrix);
}

void RenderContext::setModelMatrix(const Matrix& currentMatrix, const Matrix& prevMatrix)
{
    auto meshConstantBuffer = renderSystem.getOneshotAllocator().Allocate<MeshConstants>();
    meshConstantBuffer.Data->Model = currentMatrix;
    meshConstantBuffer.Data->PrevModel = prevMatrix;
    commandList.SetGraphicsRootConstantBufferView(Params::MeshConstantBufferView, meshConstantBuffer.GpuAddress);
}

void RenderContext::setMaterial(const Material& material, RenderPass pass)
{
    auto effect = pass == RenderPass::Shadow ? material.ShadowEffect : material.Effect;
    setEffect(*effect);
    commandList.SetGraphicsRoot32BitConstant(Params::MaterialInlineConstants, material.Index, 0);
}

void RenderContext::setEffect(Effect& effect)
{
    commandList.SetPipelineState(effect.getPipelineState()->getInternalObject()); // set effect
    effect.getPipelineState()->use();
}

void RenderContext::drawMesh(Mesh* mesh)
{
    commandList.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandList.IASetVertexBuffers(0, 1, &mesh->vertexBuffer->vertexBufferView);
    commandList.IASetIndexBuffer(&mesh->indexBuffer->indexBufferView);

    commandList.DrawIndexedInstanced(mesh->indexBuffer->indices.count(), 1, 0, 0, 0);
}

void RenderContext::drawLines(LineVertexBuffer* linesVertexBuffer)
{
    commandList.IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
    commandList.IASetVertexBuffers(0, 1, &linesVertexBuffer->vertexBufferView);
    commandList.DrawInstanced(linesVertexBuffer->vertices.count(), 1, 0, 0);
}

void RenderContext::drawFullScreenQuad()
{
    //
}
