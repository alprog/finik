export module OneshotAllocator;

import Finik.Core;
import DX;
import MemoryPage;
import Allocation;

export namespace finik::gpumem
{
    class OneshotAllocator
    {
    public:
        explicit OneshotAllocator(ID3D12Device* device);
        
        RawAllocation Allocate(int size, int frame);

        template <typename T>
        Allocation<T> Allocate()
        {
            const int32 frame = GetCurrentFrame();
            return Allocation<T>(Allocate(sizeof(T), frame));
        }

        void FreePages();

    private:
        int32 GetCurrentFrame();
        
        MemoryPage& CreateNewPage();

        ID3D12Device* device;

        Array<MemoryPage> pages;
    };
} 
