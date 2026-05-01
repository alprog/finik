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

    ImGui_ImplDX12_InitInfo init_info = {};
    init_info.Device = engine.getDevice().getInternal();
    init_info.CommandQueue = engine.get_command_queue().queueImpl.Get();
    init_info.NumFramesInFlight = NUM_FRAMES_IN_FLIGHT;
    init_info.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    init_info.DSVFormat = DXGI_FORMAT_UNKNOWN;
    init_info.SrvDescriptorHeap = engine.getDevice().getCommonHeap()->get();
    init_info.SrvDescriptorAllocFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE* out_cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE* out_gpu_handle) { 
        
        auto& handle = RenderSystem::GetInstance().engine.getCommonHeap()->getNextHandle();
        *out_cpu_handle = handle.getCPU();
        *out_gpu_handle = handle.getGPU();
    };
    init_info.SrvDescriptorFreeFn = [](ImGui_ImplDX12_InitInfo*, D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle, D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle) { 
        // TODO: NOT IMPLEMENTED
    };
    ImGui_ImplDX12_Init(&init_info);
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