export module MemoryPage;

import Finik.Core;
import DX;
import RenderSystemFwd;
import Allocation;

export namespace finik::gpumem
{   
    class MemoryPage
    {
    public:
        MemoryPage(RenderSystem& renderSystem, int size);

        int GetFullSize() const;
        int GetAvailableSize() const;
        int GetUsingFrame() const;
        byte* GetData() const;
        ID3D12Resource* GetResource() const;
        D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const;

        RawAllocation Allocate(int size, int usingFrame);
        void Reset();

    private:
        ID3D12Resource* Resource;
        void* Data;

        int32 FullSize;
        int32 UsedSize;
        int32 UsingFrame;
    };
}
