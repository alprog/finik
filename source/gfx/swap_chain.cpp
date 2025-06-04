module;
#include "dx.h"
#include <dxgi.h>
module swap_chain;

import render_system;
import gpu_profiler;
import log;
import timer;
import desktop_window;

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
        sd.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
        sd.Scaling = DXGI_SCALING_STRETCH;
        sd.Stereo = FALSE;
    }

    RenderSystem& render_system = Single::Get<RenderSystem>();

    {
        MyPtr<IDXGIFactory3> dxgiFactory;
        MyPtr<IDXGISwapChain1> dxgiSwapChain1;

        uint32 createFactoryFlags = 0;
#if defined(_DEBUG)
        createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
        auto result = CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory));
        if (FAILED(result))
            throw;

        result = dxgiFactory->CreateSwapChainForHwnd(render_system.get_command_queue().queueImpl.Get(), window.hwnd, &sd, nullptr, nullptr, &dxgiSwapChain1);
        if (FAILED(result))
            throw;

        result = dxgiSwapChain1->QueryInterface(IID_PPV_ARGS(&swapChain));
        if (FAILED(result))
            throw;

        swapChain->SetMaximumFrameLatency(NUM_BACK_BUFFER);
        hSwapChainWaitableObject = swapChain->GetFrameLatencyWaitableObject();
    }

    CreateRenderTarget();
    CreateRenderTargets();
    CreateDepthStencil();

    for (uint32 i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
        if (render_system.get_device()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&frameContext[i].CommandAllocator)) != S_OK)
            throw;
}

SwapChain::~SwapChain()
{
    for (uint32 i = 0; i < NUM_FRAMES_IN_FLIGHT; i++)
        if (frameContext[i].CommandAllocator)
        {
            frameContext[i].CommandAllocator->Release();
            frameContext[i].CommandAllocator = nullptr;
        }

    if (swapChain)
    {
        swapChain->SetFullscreenState(false, nullptr);
    }
    if (hSwapChainWaitableObject != nullptr)
    {
        CloseHandle(hSwapChainWaitableObject);
    }
}

void SwapChain::CreateRenderTargets()
{
    RenderSystem& render_system = Single::Get<RenderSystem>();

    for (uint32 i = 0; i < NUM_BACK_BUFFER; i++)
    {
        auto renderTarget = std::make_shared<RenderSurface>();
        DescriptorHeap* heap = render_system.getRtvHeap();
        renderTarget->handle = heap->getNextHandle().getCPU();

        ID3D12Resource* pBackBuffer = nullptr;
        swapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
        render_system.get_device()->CreateRenderTargetView(pBackBuffer, nullptr, renderTarget->handle);
        renderTarget->resource.InternalResource = pBackBuffer;

        renderTargets.append(renderTarget);
    }
}

void SwapChain::CreateRenderTarget()
{
    renderTarget = std::make_shared<RenderSurface>();

    CD3DX12_RESOURCE_DESC resourceDesc(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D, 0,
        static_cast<uint32>(window.width),
        static_cast<uint32>(window.height),
        1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

    RenderSystem& render_system = Single::Get<RenderSystem>();

    D3D12_CLEAR_VALUE clearValue;
    clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    renderTarget->resource.reinit(resourceDesc, D3D12_RESOURCE_STATE_RENDER_TARGET, &clearValue);
}

void SwapChain::CreateDepthStencil()
{
    CD3DX12_RESOURCE_DESC resourceDesc(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D, 0,
        static_cast<uint32>(window.width),
        static_cast<uint32>(window.height),
        1, 1, DXGI_FORMAT_D32_FLOAT, 1, 0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL | D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);

    D3D12_CLEAR_VALUE clearValue;
    clearValue.Format = DXGI_FORMAT_D32_FLOAT;
    clearValue.DepthStencil.Depth = 1.0f;
    clearValue.DepthStencil.Stencil = 0;

    RenderSystem& render_system = Single::Get<RenderSystem>();

    auto result = render_system.get_device()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &clearValue,
        IID_PPV_ARGS(&depthStencil));
    if (FAILED(result))
        throw;

    depthStencil->SetName(L"DepthStencil");

    auto device = render_system.get_device();
    depthStencilHandle = render_system.getDsvHeap()->getNextHandle();
    device->CreateDepthStencilView(depthStencil.Get(), nullptr, depthStencilHandle.getCPU());
}

void SwapChain::CleanupRenderTarget()
{
    WaitForLastSubmittedFrame();
}

void SwapChain::WaitForLastSubmittedFrame()
{
    //    FrameContext* frameCtx = &frameContext[frameIndex % NUM_FRAMES_IN_FLIGHT];
    //
    //    uint64 fenceValue = frameCtx->FenceValue;
    //    if (fenceValue == 0)
    //        return; // No fence was signaled
    //
    //    frameCtx->FenceValue = 0;
    //    if (fence->GetCompletedValue() >= fenceValue)
    //        return;
    //
    //    fence->SetEventOnCompletion(fenceValue, fenceEvent);
    //    WaitForSingleObject(fenceEvent, INFINITE);
}

FrameContext* SwapChain::WaitForNextFrameResources()
{
    uint32 nextFrameIndex = frameIndex + 1;
    frameIndex = nextFrameIndex;

    //HANDLE waitableObjects[] = { hSwapChainWaitableObject, nullptr };
    //DWORD numWaitableObjects = 1;

    RenderSystem& render_system = Single::Get<RenderSystem>();

    FrameContext* frameCtx = &frameContext[nextFrameIndex % NUM_FRAMES_IN_FLIGHT];
    uint64 fenceValue = frameCtx->FenceValue;
    if (fenceValue != 0) // means no fence was signaled
    {
        frameCtx->FenceValue = 0;
        render_system.get_command_queue().fence->WaitForValue(fenceValue);
        //waitableObjects[1] = fenceEvent;
        //numWaitableObjects = 2;
    }

    //WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

    return frameCtx;
}

void SwapChain::start_frame(ID3D12GraphicsCommandList* command_list)
{
    RenderSystem& render_system = Single::Get<RenderSystem>();

    current_frame_ctx = WaitForNextFrameResources();

    uint32 backBufferIdx = swapChain->GetCurrentBackBufferIndex();
    current_frame_ctx->CommandAllocator->Reset();

    command_list->Reset(current_frame_ctx->CommandAllocator, nullptr);
    //render_system.getProfiler()->addStamp(*command_list, "start");

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = renderTargets[backBufferIdx]->resource.InternalResource;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    command_list->ResourceBarrier(1, &barrier);

    // Render Dear ImGui graphics
    const float clear_color_with_alpha[4] = {0.2f, 0.2f, 0.2f, 1.0f};

    D3D12_CPU_DESCRIPTOR_HANDLE handle = renderTargets[backBufferIdx]->handle;
    command_list->ClearRenderTargetView(handle, clear_color_with_alpha, 0, nullptr);
    command_list->ClearDepthStencilView(depthStencilHandle.getCPU(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
    command_list->OMSetRenderTargets(1, &handle, FALSE, &depthStencilHandle.getCPU());

    ID3D12DescriptorHeap* heap = render_system.getCommonHeap()->get();
    command_list->SetDescriptorHeaps(1, &heap);

    viewport.Width = static_cast<float>(1024);
    viewport.Height = static_cast<float>(800);
    viewport.MaxDepth = 1.0f;
    command_list->RSSetViewports(1, &viewport);

    scissorRect.right = static_cast<LONG>(1024);
    scissorRect.bottom = static_cast<LONG>(800);
    command_list->RSSetScissorRects(1, &scissorRect);
}

void SwapChain::finish_frame(ID3D12GraphicsCommandList* command_list)
{
    uint32 backBufferIdx = swapChain->GetCurrentBackBufferIndex();

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = renderTargets[backBufferIdx]->resource.InternalResource;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    command_list->ResourceBarrier(1, &barrier);

    //App::GetInstance().render_system.getProfiler()->addStamp(*command_list, "end");

    command_list->Close();
}

void SwapChain::execute(ID3D12GraphicsCommandList* command_list)
{
    RenderSystem& render_system = Single::Get<RenderSystem>();
    render_system.get_command_queue()->ExecuteCommandLists(1, (ID3D12CommandList* const*)&command_list);
}

void SwapChain::present()
{
    RenderSystem& render_system = Single::Get<RenderSystem>();

    bool vsyncEnabled = true;
    HRESULT hr = swapChain->Present(vsyncEnabled ? 1 : 0, 0);

    swapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);

    uint64 fenceValue = render_system.get_command_queue().fence->SignalNext();
    current_frame_ctx->FenceValue = fenceValue;
}