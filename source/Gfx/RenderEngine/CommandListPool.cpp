module RenderEngine:CommandListPool;

import App;
import :RenderEngine;
import :CommandList;
import :GpuProfiler;

CommandListPool::CommandListPool(RenderEngine& engine, GpuProfiler& profiler)
    : engine{engine}
    , profiler{profiler}
{
}

CommandList& CommandListPool::retrieveOne()
{
    auto frameIndex = App::GetInstance().getFrameIndex();
    
    if (freeLists.empty())
    {
        return *lists.emplace_back(MakeUnique<CommandList>(engine, *this, frameIndex));
    }

    auto& list = *freeLists.last();
    freeLists.remove_last();
    list.reset(frameIndex);
    return list;
}

void CommandListPool::putBack(CommandList& commandList)
{
    freeLists.append(&commandList);
}

GpuProfiler& CommandListPool::getProfiler()
{
    return profiler;
}