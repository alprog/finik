module;
#include "gfx/dx.h"
module RenderEngine:CommandList;

import RenderEngine;
import RenderSystem;

CommandList::CommandList(CommandListPool& pool, const int32 frameIndex)
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

void CommandList::reset(const int32 frameIndex)
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

int32 CommandList::getFrameIndex() const
{
    return frameIndex;
}

void CommandList::startRecording(const char* label)
{
    if (label)
    {
        startTimebox(label);
    }
}

void CommandList::endRecording()
{
    if (!gpuTimeboxIndices.empty())
    {
        endTimebox();
    }
    listImpl->Close();
}

void CommandList::startTimebox(const char* label)
{
    auto* profiler = pool.getEngine().getProfiler();
    int32 index = profiler->startTimebox(*listImpl.Get(), label);
    gpuTimeboxIndices.append(index);
}

void CommandList::endTimebox()
{
    auto* profiler = pool.getEngine().getProfiler();
    profiler->endTimebox(*listImpl.Get(), gpuTimeboxIndices.last());
    gpuTimeboxIndices.remove_last();
}

int CommandList::addTimestampQuery()
{
    return pool.getEngine().getProfiler()->addStamp(*listImpl.Get());
}

void CommandList::transition(GpuResource& resource, D3D12_RESOURCE_STATES newState)
{
    resource.transition(newState, listImpl.Get());
}
