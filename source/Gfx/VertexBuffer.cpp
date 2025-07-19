module;
#include "dx.h"
module VertexBuffer;

import RenderSystem;

void BaseVertexBuffer::Load()
{
    auto device = Single::Get<RenderSystem>().get_device();

    const uint32 vertexBufferSize = GetVertexSize() * GetCount();

    auto result = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
                                                  &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBufferResource));

    if (FAILED(result))
        throw;

    uint8* dstData;
    CD3DX12_RANGE readRange(0, 0);

    result = vertexBufferResource->Map(0, &readRange, reinterpret_cast<void**>(&dstData));
    if (FAILED(result))
        throw;
    std::memcpy(dstData, GetData(), vertexBufferSize);
    vertexBufferResource->Unmap(0, nullptr);

    vertexBufferView.BufferLocation = vertexBufferResource->GetGPUVirtualAddress();
    vertexBufferView.StrideInBytes = GetVertexSize();
    vertexBufferView.SizeInBytes = vertexBufferSize;
}
