module;
#include "dx.h"
module vertex_buffer;

import render_system;

void VertexBuffer::Load()
{
    auto device = Single::Get<RenderSystem>().get_device();

    const uint32 vertexBufferSize = sizeof(StandardVertex) * vertices.count();

    auto result = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
                                                  &CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertexBufferResource));

    if (FAILED(result))
        throw;

    uint8* data;
    CD3DX12_RANGE readRange(0, 0);

    result = vertexBufferResource->Map(0, &readRange, reinterpret_cast<void**>(&data));
    if (FAILED(result))
        throw;
    std::memcpy(data, &vertices[0], vertexBufferSize);
    vertexBufferResource->Unmap(0, nullptr);

    vertexBufferView.BufferLocation = vertexBufferResource->GetGPUVirtualAddress();
    vertexBufferView.StrideInBytes = sizeof(StandardVertex);
    vertexBufferView.SizeInBytes = vertexBufferSize;
}
