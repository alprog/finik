module;
#include "dx.h"
module SwapChainBackBuffer;

import RenderSystem;

SwapChainBackBuffer::SwapChainBackBuffer()
{
    auto& engine = Single::Get<RenderSystem>().engine;
    descriptorHandle = engine.getRtvHeap()->getNextHandle();
}

SwapChainBackBuffer::~SwapChainBackBuffer()
{
    releaseInternal(false); // skip asserts
}

void SwapChainBackBuffer::set(IDXGISwapChain3& swapChain, int32 bufferIndex)
{
    releaseInternal(false); // skip asserts

    swapChain.GetBuffer(bufferIndex, IID_PPV_ARGS(&internalResource));
    internalResource->AddRef();

    auto& engine = Single::Get<RenderSystem>().engine;
    engine.getDevice()->CreateRenderTargetView(internalResource, nullptr, descriptorHandle.getCPU());

    state = D3D12_RESOURCE_STATE_PRESENT;
}