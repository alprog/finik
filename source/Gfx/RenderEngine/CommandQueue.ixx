export module RenderEngine:CommandQueue;

import :Shared;

export class CommandQueue
{
public:
    CommandQueue(RenderEngine& engine);

    void execute(CommandList& commandList);
    void freeCompletedLists();

    ID3D12CommandQueue* operator->();
    
    void Flush();

    RenderEngine& engine;

    MyPtr<ID3D12CommandQueue> queueImpl;

    UniquePtr<Fence> fence;      // value = incremental progress
    UniquePtr<Fence> frameFence; // value = completed frame index

    std::queue<CommandList*> ExecutionQueue;
};
