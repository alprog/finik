module;
#include "dx.h"
module command_queue;

import render_system;
import command_list;

CommandQueue::CommandQueue(RenderSystem& renderSystem)
{
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    auto result = renderSystem.get_device()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queueImpl));
    if (FAILED(result)) throw;

    frameFence = std::make_unique<Fence>(renderSystem, *queueImpl.Get());
    fence = std::make_unique<Fence>(renderSystem, *queueImpl.Get());
}

ID3D12CommandQueue* CommandQueue::operator->() 
{
    return queueImpl.Get(); 
}

void CommandQueue::execute(CommandList& commandList)
{
    executionQueue.push(&commandList);
    queueImpl->ExecuteCommandLists(1, (ID3D12CommandList* const*)&commandList);
}

void CommandQueue::freeCompletedLists()
{
    auto completedFrameIndex = frameFence->GetCompletedValue();

    while (!executionQueue.empty())
    {
        if (executionQueue.front()->getFrameIndex() <= completedFrameIndex)
        {
            executionQueue.front()->returnToPool();
            executionQueue.pop();
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
