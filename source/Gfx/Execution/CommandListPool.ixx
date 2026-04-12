export module Execution:CommandListPool;

import :Shared;
import :CommandList;
import :GpuProfiler;

export class CommandListPool
{
public:
    explicit CommandListPool(GfxDevice& device, GpuProfiler& profiler);

    CommandList& retrieveOne();
    void putBack(CommandList& commandList);

    GpuProfiler& getProfiler();

private:
    GfxDevice& device;
    GpuProfiler& profiler;

    Array<UniquePtr<CommandList>> lists;
    Array<CommandList*> freeLists;
};
