module;
#include "dx.h"
module RenderSystem;

import App;
import Imgui;
import RenderEngine;
import GfxDevice;
import DesktopWindow;

RenderSystem::RenderSystem()
{
    init();
}

void RenderSystem::init()
{
    engine.init();
    createRenderContext();
}

void RenderSystem::ImguiInitHelper()
{
    const int NUM_FRAMES_IN_FLIGHT = 3;

    DescriptorHandle handle = engine.getDevice().getCommonHeap()->getNextHandle();
    ImGui_ImplDX12_Init(getInternalDevice(), NUM_FRAMES_IN_FLIGHT, DXGI_FORMAT_R8G8B8A8_UNORM, engine.getDevice().getCommonHeap()->get(), handle.getCPU(), handle.getGPU());
}

GfxDevice& RenderSystem::getDevice()
{
    return engine.getDevice();
}

ID3D12Device* RenderSystem::getInternalDevice()
{
    return engine.getInternalDevice();
}

RenderContext* RenderSystem::getRenderContext()
{
    return renderContext.get();
}

GpuProfiler* RenderSystem::getProfiler()
{
    return engine.getProfiler();
}

MainRootSignature& RenderSystem::getRootSignature()
{
    return engine.getRootSignature();
}

ComputeRootSignature& RenderSystem::getComputeRootSignature()
{
    return engine.getComputeRootSignature();
}

void RenderSystem::createRenderContext()
{
    renderContext = MakeUnique<RenderContext>(engine, *engine.get_command_list());
}

void RenderSystem::scheduleQueryResolving()
{
    engine.scheduleQueryResolving();
}