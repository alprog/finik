export module DescriptorHandle;

import Finik.Core;
import DX;
import RenderSystemFwd;

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
