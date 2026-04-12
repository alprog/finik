module;
#include "gfx/dx.h"
module Execution:CommandList;

import RenderSystem;
import :CommandListPool;
import :GpuProfiler;
import GpuResource;

CommandList::CommandList(GfxDevice& device, CommandListPool& pool, const int frameIndex)
    : device{device}
    , pool{pool}
    , frameIndex{frameIndex}
{
    auto result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));

    if (FAILED(result))
        throw;

    result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&listImpl));
    if (FAILED(result))
        throw;

    // open and ready to record
}

void CommandList::reset(const int frameIndex)
{
    commandAllocator->Reset();
    listImpl->Reset(commandAllocator.Get(), nullptr);
    this->frameIndex = frameIndex;
}

void CommandList::returnToPool()
{
    frameIndex = 0;
    pool.putBack(*this);
}

int CommandList::getFrameIndex() const
{
    return frameIndex;
}

void CommandList::startRecording()
{
    startTimestampIndex = addTimestampQuery();
}

void CommandList::endRecording()
{
    endTimestampIndex = addTimestampQuery();
    listImpl->Close();
}

int CommandList::addTimestampQuery()
{
    auto& renderSystem = Single::Get<RenderSystem>();
    return renderSystem.getProfiler()->addStamp(*listImpl.Get(), "list");
}

void CommandList::transition(GpuResource& resource, D3D12_RESOURCE_STATES newState)
{
    if (resource.state != newState)
    {
        listImpl->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource.getInternal(), resource.state, newState));
        resource.state = newState;
    }
}
