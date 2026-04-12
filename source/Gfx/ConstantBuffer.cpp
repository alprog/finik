module;
#include "dx.h"
module ConstantBuffer;

import UploadBuffer;

GpuDataBuffer::GpuDataBuffer(int32 size, GfxDevice& device)
    : size{size}
{
    const int32 alignedSize = (sizeof(size) + 255) & ~255; // align 256

    uploadBuffer = new UploadBuffer(device, alignedSize);

    D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
    cbvDesc.BufferLocation = uploadBuffer->GetGPUVirtualAddress();
    cbvDesc.SizeInBytes = alignedSize;

    data = (uint8*)uploadBuffer->GetData();

    descriptorHandle = device.getCommonHeap()->getNextHandle();
    device->CreateConstantBufferView(&cbvDesc, descriptorHandle.getCPU());
}

GpuDataBuffer::~GpuDataBuffer()
{
    delete uploadBuffer;
}
