export module Descriptors;

import Finik.Core;
import DX;

class DescriptorHeap;

export struct DescriptorHandle
{
public:
    DescriptorHandle();
    DescriptorHandle(DescriptorHeap* heap, int32 index);

    int getIndex() const;

    CD3DX12_CPU_DESCRIPTOR_HANDLE getCPU() const;
    CD3DX12_GPU_DESCRIPTOR_HANDLE getGPU() const;

private:
    DescriptorHeap* heap;
    int index;
};

//-----------------------------------------------------------------------------------------------

export class DescriptorHeap
{
public:
    DescriptorHeap(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, unsigned int maxCount);

    ID3D12DescriptorHeap* get()
    {
        return heap.Get();
    }
    DescriptorHandle getNextHandle();

    inline CD3DX12_CPU_DESCRIPTOR_HANDLE getCpuHandle(int index)
    {
        return {cpuStartHandle, index * handleIncrementSize};
    }

    inline CD3DX12_GPU_DESCRIPTOR_HANDLE getGpuHandle(int index)
    {
        return {gpuStartHandle, index * handleIncrementSize};
    }

private:
    MyPtr<ID3D12DescriptorHeap> heap;

    CD3DX12_CPU_DESCRIPTOR_HANDLE cpuStartHandle;
    CD3DX12_GPU_DESCRIPTOR_HANDLE gpuStartHandle;
    int handleIncrementSize;

    int descriptorCount;
    int maxCount;
};
