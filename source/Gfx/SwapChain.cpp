module;
#include "dx.h"
#include <dxgi.h>
module SwapChain;

import RenderSystem;
import RenderEngine;
import Log;
import Timer;
import DesktopWindow;
import App;

D3D12_VIEWPORT viewport;
D3D12_RECT scissorRect;

SwapChain::SwapChain(DesktopWindow& window)
    : window{window}
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC1 sd;
    {
        ZeroMemory(&sd, sizeof(sd));
        sd.BufferCount = NUM_BACK_BUFFER;
        sd.Width = window.width;
        sd.Height = window.height;
        sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.Flags = 0; //DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        sd.Scaling = DXGI_SCALING_STRETCH;
        sd.Stereo = FALSE;
    }
    
    resolution = {window.width, window.height};

    auto& engine = Single::Get<RenderSystem>().engine;

    MyPtr<IDXGIFactory3> dxgiFactory;
    MyPtr<IDXGISwapChain1> dxgiSwapChain1;

    uint32 createFactoryFlags = 0;
#if defined(_DEBUG)
    createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
    auto result = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
    if (FAILED(result))
        throw;

    result = dxgiFactory->CreateSwapChainForHwnd(engine.get_command_queue().queueImpl.Get(), window.hwnd, &sd, nullptr, nullptr, &dxgiSwapChain1);
    if (FAILED(result))
        throw;

    result = dxgiSwapChain1->QueryInterface(IID_PPV_ARGS(&swapChain));
    if (FAILED(result))
        throw;

    swapChain->SetMaximumFrameLatency(NUM_BACK_BUFFER);
    hSwapChainWaitableObject = swapChain->GetFrameLatencyWaitableObject();
  
    createBackBuffers();
}

SwapChain::~SwapChain()
{
    if (swapChain)
    {
        swapChain->SetFullscreenState(false, nullptr);
    }
    if (hSwapChainWaitableObject != nullptr)
    {
        CloseHandle(hSwapChainWaitableObject);
    }
}

void SwapChain::resize()
{
    IntSize windowSize {window.width, window.height};
    if (this->resolution != windowSize)
    {
        auto& engine = Single::Get<RenderSystem>().engine;
        engine.get_command_queue().Flush();
    
        for (auto& backBuffer : backBuffers)
        {
            backBuffer->reset();
        }

        engine.get_command_queue().Flush();

        DXGI_SWAP_CHAIN_DESC desc = {};
        swapChain->GetDesc(&desc);
        swapChain->ResizeBuffers(NUM_BACK_BUFFER, window.width, window.height, desc.BufferDesc.Format, desc.Flags) MUST;
    
        for (int32 i = 0; i < backBuffers.count(); i++)
        {
            backBuffers[i]->set(*swapChain.Get(), i);
        }

        this->resolution = windowSize;
    }
}

void SwapChain::createBackBuffers()
{
    auto& engine = Single::Get<RenderSystem>().engine;

    for (uint32 i = 0; i < NUM_BACK_BUFFER; i++)
    {
        auto backBuffer = MakePtr<SwapChainBackBuffer>();
        backBuffer->set(*swapChain.Get(), i);
        backBuffers.append(backBuffer);
    }
}

void SwapChain::WaitForNextFrameResources()
{
    if (presentFenceValues.size() >= NUM_FRAMES_IN_FLIGHT)
    {
        uint64 fenceValue = presentFenceValues.front();
        auto& engine = Single::Get<RenderSystem>().engine;
        engine.get_command_queue().fence->WaitForValue(fenceValue);
        presentFenceValues.pop();
    }
}

void SwapChain::start_frame(CommandList& list)
{
    WaitForNextFrameResources();

    list.startRecording();

    auto command_list = list.listImpl.Get();
    auto& engine = Single::Get<RenderSystem>().engine;

    uint32 backBufferIdx = swapChain->GetCurrentBackBufferIndex();
    //engine.getProfiler()->addStamp(*command_list, "start");

    list.transition(*backBuffers[backBufferIdx], D3D12_RESOURCE_STATE_RENDER_TARGET);

    // Render Dear ImGui graphics
    const float clear_color_with_alpha[4] = {0.2f, 0.2f, 0.2f, 1.0f};

    D3D12_CPU_DESCRIPTOR_HANDLE handle = backBuffers[backBufferIdx]->descriptorHandle.getCPU();
    command_list->ClearRenderTargetView(handle, clear_color_with_alpha, 0, nullptr);
    //command_list->ClearDepthStencilView(depthStencilHandle.getCPU(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
    command_list->OMSetRenderTargets(1, &handle, FALSE, nullptr);

    ID3D12DescriptorHeap* heap = engine.getCommonHeap()->get();
    command_list->SetDescriptorHeaps(1, &heap);
}

void SwapChain::finish_frame(CommandList& list)
{
    auto command_list = list.listImpl.Get();

    uint32 backBufferIdx = swapChain->GetCurrentBackBufferIndex();

    list.transition(*backBuffers[backBufferIdx], D3D12_RESOURCE_STATE_PRESENT);

    //App::GetInstance().render_system.getProfiler()->addStamp(*command_list, "end");

    list.endRecording();
}

void SwapChain::execute(CommandList& list)
{
    auto& engine = Single::Get<RenderSystem>().engine;
    engine.get_command_queue().execute(list);
}

void SwapChain::present()
{
    auto& engine = Single::Get<RenderSystem>().engine;

    bool vsyncEnabled = true;
    HRESULT hr = swapChain->Present(vsyncEnabled ? 1 : 0, 0);

    swapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);

    uint64 fenceValue = engine.get_command_queue().fence->SignalNext();
    presentFenceValues.push(fenceValue);
}
