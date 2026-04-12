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

    MyPtr<ID3D12CommandQueue> queueImpl;
    UniquePtr<Fence> fence;

    UniquePtr<Fence> frameFence;

    std::queue<CommandList*> ExecutionQueue;
};
