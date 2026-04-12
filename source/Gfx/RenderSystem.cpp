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

CommandQueue& RenderSystem::get_command_queue()
{
    return engine.get_command_queue();
}

ID3D12GraphicsCommandList* RenderSystem::get_command_list()
{
    return engine.get_command_list();
}

DescriptorHeap* RenderSystem::getRtvHeap()
{
    return engine.getRtvHeap();
}

DescriptorHeap* RenderSystem::getDsvHeap()
{
    return engine.getDsvHeap();
}

DescriptorHeap* RenderSystem::getCommonHeap()
{
    return engine.getCommonHeap();
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

CommandList& RenderSystem::getFreeCommandList()
{
    return engine.getFreeCommandList();
}

finik::gpumem::OneshotAllocator& RenderSystem::getOneshotAllocator()
{
    return engine.getOneshotAllocator();
}

void RenderSystem::createRenderContext()
{
    renderContext = MakeUnique<RenderContext>(*this, *engine.get_command_list());
}

void RenderSystem::scheduleQueryResolving()
{
    engine.scheduleQueryResolving();
}