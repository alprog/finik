module;
#include "dx.h"
module RenderSystem;

import App;
import Imgui;
import RenderEngine;
import GfxDevice;
import DesktopWindow;

class ResultChecker
{
public:
    ResultChecker() = default;

    ResultChecker(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw std::exception();
        }
    }

    void operator|=(HRESULT hr)
    {
        if (FAILED(hr))
        {
            throw std::exception();
        }
    }
};

static int const NUM_BACK_BUFFERS = 3;

RenderSystem::RenderSystem()
{
    init();
}

void RenderSystem::init()
{
    createDevice();
    createCommandQueue();
    createProfiler();
    createCommandListPool();
    createCommandAllocators();
    createCommandList();
    createRenderContext();
    createRootSignature();
}

void RenderSystem::createDevice()
{
    engine.device.init();
}

void RenderSystem::createCommandQueue()
{
    commandQueue = new CommandQueue(engine);
}

void RenderSystem::createCommandListPool()
{
    commandListPool = MakeUnique<CommandListPool>(engine, *gpuProfiler);
}

void RenderSystem::createProfiler()
{
    gpuProfiler = new GpuProfiler(engine, *commandQueue);
}

void RenderSystem::createCommandAllocators()
{
    for (int i = 0; i < 3; i++)
    {
        auto result = engine.device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocators[i]));
        if (FAILED(result))
            throw;
    }
}

void RenderSystem::createCommandList()
{
    auto result = getInternalDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocators[0].Get(), nullptr, IID_PPV_ARGS(&commandList));
    if (FAILED(result))
        throw;

    result = commandList->Close();
    if (FAILED(result))
        throw;
}

void RenderSystem::createRenderContext()
{
    renderContext = MakeUnique<RenderContext>(*this, *commandList.Get());
}

void RenderSystem::createRootSignature()
{
    mainRootSignature = MakeUnique<MainRootSignature>(engine.device);
    computeRootSignature = MakeUnique<ComputeRootSignature>(engine.device);
}

void RenderSystem::scheduleQueryResolving()
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

void RenderSystem::ImguiInitHelper()
{
    const int NUM_FRAMES_IN_FLIGHT = 3;

    DescriptorHandle handle = engine.device.getCommonHeap()->getNextHandle();
    ImGui_ImplDX12_Init(getInternalDevice(), NUM_FRAMES_IN_FLIGHT, DXGI_FORMAT_R8G8B8A8_UNORM, engine.device.getCommonHeap()->get(), handle.getCPU(), handle.getGPU());
}

GfxDevice& RenderSystem::getDevice()
{
    return engine.device;
}

ID3D12Device* RenderSystem::getInternalDevice()
{
    return engine.device.getInternal();
}

CommandQueue& RenderSystem::get_command_queue()
{
    return *commandQueue;
}

ID3D12GraphicsCommandList* RenderSystem::get_command_list()
{
    return commandList.Get();
}

DescriptorHeap* RenderSystem::getRtvHeap()
{
    return engine.device.getRtvHeap();
}

DescriptorHeap* RenderSystem::getDsvHeap()
{
    return engine.device.getDsvHeap();
}

DescriptorHeap* RenderSystem::getCommonHeap()
{
    return engine.device.getCommonHeap();
}

RenderContext* RenderSystem::getRenderContext()
{
    return renderContext.get();
}

GpuProfiler* RenderSystem::getProfiler()
{
    return gpuProfiler;
}

MainRootSignature& RenderSystem::getRootSignature()
{
    return *mainRootSignature;
}

ComputeRootSignature& RenderSystem::getComputeRootSignature()
{
    return *computeRootSignature;
}

CommandList& RenderSystem::getFreeCommandList()
{
    return commandListPool->retrieveOne();
}

finik::gpumem::OneshotAllocator& RenderSystem::getOneshotAllocator()
{
    return engine.device.getOneshotAllocator();
}
