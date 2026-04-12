module OneshotAllocator;

import RenderSystem;
import App;

using namespace finik::gpumem;

OneshotAllocator::OneshotAllocator(ID3D12Device* device)
    : device{ device }
{
}

RawAllocation OneshotAllocator::Allocate(const int size, const int frame)
{
    constexpr int32 alignmentMask = 0xFF;
    const int32 allignedSize = (size + alignmentMask) & ~alignmentMask;

    for (auto& page : pages)
    {
        if (page.GetAvailableSize() >= allignedSize)
        {
            return page.Allocate(allignedSize, frame);
        }
    }

    return CreateNewPage().Allocate(allignedSize, frame);
}

void OneshotAllocator::FreePages()
{
    auto currentFrame = GetCurrentFrame();
    for (auto& page : pages)
    {
        if (page.GetUsingFrame() < currentFrame)
        {
            page.Reset();
        }
    }
}

MemoryPage& OneshotAllocator::CreateNewPage()
{
    return pages.emplace_back(device, 4096);
}

int32 OneshotAllocator::GetCurrentFrame()
{
    return App::GetInstance().getFrameIndex();
}
