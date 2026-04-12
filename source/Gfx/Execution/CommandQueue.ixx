export module Execution:CommandQueue;

import :Shared;
import :Fence;
import :CommandList;

export class CommandQueue
{
public:
    CommandQueue(GfxDevice& device);

    void execute(CommandList& commandList);
    void freeCompletedLists();

    ID3D12CommandQueue* operator->();
    
    void Flush();

    MyPtr<ID3D12CommandQueue> queueImpl;
    UniquePtr<Fence> fence;

    UniquePtr<Fence> frameFence;

    std::queue<CommandList*> executionQueue;
};
