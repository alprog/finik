export module RenderEngine:CommandListPool;

import :Shared;

export class GpuProfiler;
export class CommandList;

export class CommandListPool
{
public:
    explicit CommandListPool(RenderEngine& engine, GpuProfiler& profiler);

    CommandList& retrieveOne();
    void putBack(CommandList& commandList);

    GpuProfiler& getProfiler();

private:
    RenderEngine& engine;
    GpuProfiler& profiler;

    Array<UniquePtr<CommandList>> lists;
    Array<CommandList*> freeLists;
};
