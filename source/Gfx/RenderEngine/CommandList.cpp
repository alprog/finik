module;
#include "gfx/dx.h"
module RenderEngine:CommandList;

import RenderEngine;
import RenderSystem;

CommandList::CommandList(CommandListPool& pool, const int frameIndex)
    : pool{pool}
    , frameIndex{frameIndex}
{
    auto& device = pool.getEngine().getDevice();
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
    gpuTimeboxIndex = pool.getEngine().getProfiler()->startTimebox(*listImpl.Get(), "list");
}

void CommandList::endRecording()
{
    pool.getEngine().getProfiler()->endTimebox(*listImpl.Get(), gpuTimeboxIndex);
    listImpl->Close();
}

int CommandList::addTimestampQuery()
{
    return pool.getEngine().getProfiler()->addStamp(*listImpl.Get());
}

void CommandList::transition(GpuResource& resource, D3D12_RESOURCE_STATES newState)
{
    resource.transition(newState, listImpl.Get());
}
