export module OneshotAllocator;

import Finik.Core;
import MemoryPage;
import RenderSystemFwd;
import Allocation;

export namespace finik::gpumem
{
    class OneshotAllocator
    {
    public:
        explicit OneshotAllocator(RenderSystem& renderSystem);
        
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

        RenderSystem& renderSystem;

        Array<MemoryPage> pages;
    };
} 
