module;
#include "dx.h"
module constant_buffer;

import render_system;
import upload_buffer;

GpuDataBuffer::GpuDataBuffer(int32 size, RenderSystem& renderSystem)
    : size{size}
{
    const int32 alignedSize = (sizeof(size) + 255) & ~255; // align 256

    uploadBuffer = new UploadBuffer(renderSystem, alignedSize);

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = uploadBuffer->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = alignedSize;

    data = (uint8*)uploadBuffer->GetData();

    descriptorHandle = renderSystem.getCommonHeap()->getNextHandle();
    renderSystem.get_device()->CreateConstantBufferView(&cbvDesc, descriptorHandle.getCPU());
}

GpuDataBuffer::~GpuDataBuffer()
{
    delete uploadBuffer;
}
