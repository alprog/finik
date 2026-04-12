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
    createCommandAllocators();
    createCommandList();
    createRootSignature();
}

GfxDevice& RenderEngine::getDevice()
{
    return device;
}

ID3D12Device* RenderEngine::getInternalDevice()
{
    return device.getInternal();
}

CommandQueue& RenderEngine::get_command_queue()
{
    return *commandQueue;
}

ID3D12GraphicsCommandList* RenderEngine::get_command_list()
{
    return commandList.Get();
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

void RenderEngine::createCommandAllocators()
{
    for (int i = 0; i < 3; i++)
    {
        auto result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocators[i]));
        if (FAILED(result))
            throw;
    }
}

void RenderEngine::createCommandList()
{
    auto result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(&commandList));
    if (FAILED(result))
        throw;

    result = commandList->Close();
    if (FAILED(result))
        throw;
}

void RenderEngine::createRootSignature()
{
    mainRootSignature = MakeUnique<MainRootSignature>(device);
    computeRootSignature = MakeUnique<ComputeRootSignature>(device);
}

void RenderEngine::scheduleQueryResolving()
{
    static int i = 0;
    commandList->Reset(commandAllocators[i].Get(), nullptr);
    //gpuProfiler->addStamp(*commandList.Get(), "resolving");
    gpuProfiler->scheduleFrameResolve(*commandList.Get());
    commandList->Close();

    ID3D12GraphicsCommandList* command_list = commandList.Get();
    get_command_queue()->ExecuteCommandLists(1, (ID3D12CommandList* const*)&command_list);

    auto fenceValue = (int32)get_command_queue().fence->SignalNext();
    gpuProfiler->endFrameRange(fenceValue);

    i = (i + 1) % 3;
}
