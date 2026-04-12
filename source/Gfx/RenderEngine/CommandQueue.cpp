module;
#include "../dx.h"
module RenderEngine:CommandQueue;

import RenderSystem;

CommandQueue::CommandQueue(RenderEngine& engine)
    : engine{engine}
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    auto result = engine.getDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queueImpl));
    if (FAILED(result)) throw;

    frameFence = MakeUnique<Fence>(*this);
    fence = MakeUnique<Fence>(*this);
}

ID3D12CommandQueue* CommandQueue::operator->() 
{
    return queueImpl.Get(); 
}

void CommandQueue::execute(CommandList& commandList)
{
    ExecutionQueue.push(&commandList);
    queueImpl->ExecuteCommandLists(1, (ID3D12CommandList* const*)&commandList);
}

void CommandQueue::freeCompletedLists()
{
    auto completedFrameIndex = frameFence->GetCompletedValue();

    while (!ExecutionQueue.empty())
    {
        if (ExecutionQueue.front()->getFrameIndex() <= completedFrameIndex)
        {
            ExecutionQueue.front()->returnToPool();
            ExecutionQueue.pop();
        }
        else
        {
            break;
        }
    }
}

void CommandQueue::Flush()
{
    fence->WaitForValue(fence->SignalNext());
}
