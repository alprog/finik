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

    auto& device = engine.getDevice();
    DescriptorHandle handle = device.getCommonHeap()->getNextHandle();
    ImGui_ImplDX12_Init(device.getInternal(), NUM_FRAMES_IN_FLIGHT, DXGI_FORMAT_R8G8B8A8_UNORM, device.getCommonHeap()->get(), handle.getCPU(), handle.getGPU());
}

RenderContext* RenderSystem::getRenderContext()
{
    return renderContext.get();
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