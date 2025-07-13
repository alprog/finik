export module descriptor_handle;

import Finik.Core;
import dx;
import render_system_fwd;

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
