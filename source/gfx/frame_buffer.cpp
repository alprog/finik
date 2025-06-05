module;
#include "dx.h"
module frame_buffer;

import render_system;
import enum_bits;

FrameBuffer::FrameBuffer(MRT mrt, IntSize resolution)
{
    this->renderTargetEnabled = (mrt & MRT::RT0) != MRT::None;
    this->depthStencilEnabled = (mrt & MRT::DS) != MRT::None;
    createHandles();
    resize(resolution);
}

void FrameBuffer::createHandles()
{
    RenderSystem& render_system = Single::Get<RenderSystem>();
    if (renderTargetEnabled)
    {
        renderTarget.handle = render_system.getRtvHeap()->getNextHandle();
        renderTarget.textureHandle = render_system.getCommonHeap()->getNextHandle();
    }
}

void FrameBuffer::resize(IntSize resolution)
{
    this->resolution = resolution;
    if (renderTargetEnabled)
    {
        recreateRenderTarget();
    }
    if (depthStencilEnabled)
    {
        recreateDepthStencil();
    }
}

void FrameBuffer::recreateRenderTarget()
{
    RenderSystem& render_system = Single::Get<RenderSystem>();
    CD3DX12_RESOURCE_DESC resourceDesc(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D, 0,
        static_cast<uint32>(resolution.width),
        static_cast<uint32>(resolution.height),
        1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

    D3D12_CLEAR_VALUE clearValue;
    clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    renderTarget.resource.reinit(resourceDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue);

    render_system.get_device()->CreateRenderTargetView(renderTarget.resource.getInternal(), nullptr, renderTarget.handle.getCPU());
    render_system.get_device()->CreateShaderResourceView(renderTarget.resource.getInternal(), nullptr, renderTarget.textureHandle.getCPU());
}

void FrameBuffer::recreateDepthStencil()
{
    depthStencil.resize(resolution);
}

void FrameBuffer::startRendering(CommandList& commandList)
{
    RenderSystem& render_system = Single::Get<RenderSystem>();

    if (renderTargetEnabled)
    {
        commandList.transition(renderTarget.resource, D3D12_RESOURCE_STATE_RENDER_TARGET);

        const float clear_color_with_alpha[4] = {0.5f, 0.2f, 0.2f, 1.0f};
        commandList.listImpl->ClearRenderTargetView(renderTarget.handle.getCPU(), clear_color_with_alpha, 0, nullptr);
    }
    if (depthStencilEnabled)
    {
        commandList.transition(depthStencil.resource, D3D12_RESOURCE_STATE_DEPTH_WRITE);
        commandList.listImpl->ClearDepthStencilView(depthStencil.handle.getCPU(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
    }

    commandList.listImpl->OMSetRenderTargets(
        renderTargetEnabled ? 1 : 0,
        renderTargetEnabled ? &renderTarget.handle.getCPU() : nullptr,
        FALSE,
        depthStencilEnabled ? &depthStencil.handle.getCPU() : nullptr);

    ID3D12DescriptorHeap* a = render_system.getCommonHeap()->get();
    commandList.listImpl->SetDescriptorHeaps(1, &a);

    viewport.Width = static_cast<float>(resolution.width);
    viewport.Height = static_cast<float>(resolution.height);
    viewport.MaxDepth = 1.0f;
    commandList.listImpl->RSSetViewports(1, &viewport);

    scissorRect.right = static_cast<LONG>(resolution.width);
    scissorRect.bottom = static_cast<LONG>(resolution.height);
    commandList.listImpl->RSSetScissorRects(1, &scissorRect);
}

void FrameBuffer::endRendering(CommandList& commandList)
{
    if (renderTargetEnabled)
    {
        commandList.transition(renderTarget.resource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    }
    if (depthStencilEnabled)
    {
        commandList.transition(depthStencil.resource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    }
}