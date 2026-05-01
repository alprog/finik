module;
#include "../dx.h"
module RenderEngine;

import RenderEngine;

void RenderEngine::init()
{
    createDevice();
    createCommandQueue();
    createProfiler();
    createCommandListPool();
    createRootSignature();
}

GfxDevice& RenderEngine::getDevice()
{
    return device;
}

CommandQueue& RenderEngine::get_command_queue()
{
    return *commandQueue;
}

DescriptorHeap* RenderEngine::getRtvHeap()
{
    return device.getRtvHeap();
}

DescriptorHeap* RenderEngine::getDsvHeap()
{
    return device.getDsvHeap();
}

DescriptorHeap* RenderEngine::getCommonHeap()
{
    return device.getCommonHeap();
}

GpuProfiler* RenderEngine::getProfiler()
{
    return gpuProfiler;
}

MainRootSignature& RenderEngine::getRootSignature()
{
    return *mainRootSignature;
}

ComputeRootSignature& RenderEngine::getComputeRootSignature()
{
    return *computeRootSignature;
}

CommandList& RenderEngine::getFreeCommandList()
{
    return commandListPool->retrieveOne();
}

finik::gpumem::OneshotAllocator& RenderEngine::getOneshotAllocator()
{
    return device.getOneshotAllocator();
}

void RenderEngine::createDevice()
{
    device.init();
}

void RenderEngine::createCommandQueue()
{
    commandQueue = new CommandQueue(*this);
}

void RenderEngine::createCommandListPool()
{
    commandListPool = MakeUnique<CommandListPool>(*this);
}

void RenderEngine::createProfiler()
{
    gpuProfiler = new GpuProfiler(*this);
}

void RenderEngine::createRootSignature()
{
    mainRootSignature = MakeUnique<MainRootSignature>(device);
    computeRootSignature = MakeUnique<ComputeRootSignature>(device);
}

void RenderEngine::scheduleQueryResolving()
{
    CommandList& list = commandListPool->retrieveOne();
    list.startRecording();
    gpuProfiler->scheduleFrameResolve(*list.listImpl.Get());
    list.endRecording();
    commandQueue->execute(list);
}

void RenderEngine::signalEndFrame()
{
    auto fenceValue = (int32)get_command_queue().fence->SignalNext();
    gpuProfiler->endFrameRange(fenceValue);
}