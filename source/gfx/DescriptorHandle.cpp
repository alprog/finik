module descriptor_handle;

import descriptor_heap;

DescriptorHandle::DescriptorHandle()
    : heap{nullptr}
    , index{0}
{
}

DescriptorHandle::DescriptorHandle(DescriptorHeap* heap, int index)
    : heap{heap}
    , index{index}
{
}

int DescriptorHandle::getIndex() const
{
    return index;
}

CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle::getCPU() const
{
    return heap->getCpuHandle(index);
}

CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorHandle::getGPU() const
{
    return heap->getGpuHandle(index);
}
