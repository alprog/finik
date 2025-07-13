module;
#include "gfx/dx.h"
module memory_page;

import render_system;

using namespace finik::gpumem;

MemoryPage::MemoryPage(RenderSystem& renderSystem, int size)
    : FullSize{ size }
    , UsedSize { 0 }
    , UsingFrame { 0 }
{
    auto device = renderSystem.get_device();

    static D3D12_HEAP_PROPERTIES HeapProps;
    HeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
    HeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    HeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    HeapProps.CreationNodeMask = 1;
    HeapProps.VisibleNodeMask = 1;

    auto result = device->CreateCommittedResource(
        &HeapProps,
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(size),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&Resource));

    if (FAILED(result)) throw;

    result = Resource->Map(0, nullptr, &Data);
    if (FAILED(result)) throw;

}

int MemoryPage::GetFullSize() const
{
    return FullSize;
}

int MemoryPage::GetAvailableSize() const
{
    return FullSize - UsedSize;
}

int MemoryPage::GetUsingFrame() const
{
    return UsingFrame;
}

byte* MemoryPage::GetData() const
{
    return reinterpret_cast<byte*>(Data);
}

ID3D12Resource* MemoryPage::GetResource() const
{
    return Resource;
}

D3D12_GPU_VIRTUAL_ADDRESS MemoryPage::GetGPUVirtualAddress() const
{
    return Resource->GetGPUVirtualAddress();
}

RawAllocation MemoryPage::Allocate(int size, int usingFrame)
{
    RawAllocation allocation;
    if (UsedSize + size > FullSize)
    {
        allocation.CpuData = nullptr;
    }
    else
    {
        allocation.CpuData = reinterpret_cast<byte*>(Data) + UsedSize;
        allocation.GpuAddress = GetGPUVirtualAddress() + UsedSize;

        UsedSize += size;
        UsingFrame = max(usingFrame, UsingFrame);

    }
    return allocation;
}

void MemoryPage::Reset()
{
    UsedSize = 0;
}