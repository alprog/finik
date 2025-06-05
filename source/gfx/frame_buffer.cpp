module;
#include "dx.h"
module frame_buffer;

import render_system;

FrameBuffer::FrameBuffer(IntSize resolution, int32 renderTargetCount, bool depthBufferEnabled)
{
    for (int i = 0; i < renderTargetCount; i++)
    {
        renderTargets.emplace_back(std::make_unique<RenderTarget>(resolution));
    }

    if (depthBufferEnabled)
    {
        depthStencil = std::make_unique<DepthStencil>(resolution);
    }
}

void FrameBuffer::resize(IntSize resolution)
{
    for (auto& renderTarget : renderTargets)
    {
        renderTarget->resize(resolution);
    }
    if (depthStencil)
    {
        depthStencil->resize(resolution);
    }
}

void FrameBuffer::startRendering(CommandList& commandList)
{
    RenderSystem& render_system = Single::Get<RenderSystem>();

    for (auto& renderTarget : renderTargets)
    {
        commandList.transition(renderTarget->resource, D3D12_RESOURCE_STATE_RENDER_TARGET);

        const float clear_color_with_alpha[4] = {0.5f, 0.2f, 0.2f, 1.0f};
        commandList.listImpl->ClearRenderTargetView(renderTarget->handle.getCPU(), clear_color_with_alpha, 0, nullptr);
    }
    if (depthStencil)
    {
        commandList.transition(depthStencil->resource, D3D12_RESOURCE_STATE_DEPTH_WRITE);
        commandList.listImpl->ClearDepthStencilView(depthStencil->handle.getCPU(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
    }

    commandList.listImpl->OMSetRenderTargets(
        renderTargets.count(),
        !renderTargets.empty() ? &renderTargets[0]->handle.getCPU() : nullptr,
        FALSE,
        depthStencil ? &depthStencil->handle.getCPU() : nullptr);

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
    for (auto& renderTarget : renderTargets)
    {
        commandList.transition(renderTarget->resource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    }
    if (depthStencil)
    {
        commandList.transition(depthStencil->resource, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
    }
}