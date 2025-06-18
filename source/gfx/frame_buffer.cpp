module;
#include "dx.h"
module frame_buffer;

import render_system;

FrameBuffer::FrameBuffer(SurfaceResolution resolution, int32 renderTargetCount, bool depthBufferEnabled)
    : resolution{resolution}
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

void FrameBuffer::resize(SurfaceResolution resolution)
{
    if (this->resolution != resolution)
    {
        for (auto& renderTarget : renderTargets)
        {
            renderTarget->resize(resolution);
        }
        if (depthStencil)
        {
            depthStencil->resize(resolution);
        }
        this->resolution = resolution;
    }
}

void FrameBuffer::startRendering(CommandList& commandList)
{
    RenderSystem& render_system = Single::Get<RenderSystem>();

    Array<CD3DX12_CPU_DESCRIPTOR_HANDLE> RTHandles;
    for (auto& renderTarget : renderTargets)
    {
        commandList.transition(renderTarget->resource, D3D12_RESOURCE_STATE_RENDER_TARGET);

        const float clear_color_with_alpha[4] = {0, 0, 0, 0};
        commandList.listImpl->ClearRenderTargetView(renderTarget->handle.getCPU(), clear_color_with_alpha, 0, nullptr);
    
        RTHandles.append(renderTarget->handle.getCPU());
    }
    if (depthStencil)
    {
        commandList.transition(depthStencil->resource, D3D12_RESOURCE_STATE_DEPTH_WRITE);
        commandList.listImpl->ClearDepthStencilView(depthStencil->handle.getCPU(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
    }

    commandList.listImpl->OMSetRenderTargets(
        RTHandles.count(),
        !RTHandles.empty() ? &RTHandles[0] : nullptr,
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

RenderSurface* FrameBuffer::getRenderSurface(MRT type) const
{
    if (type == MRT::DS)
    {
        if (depthStencil)
        {
            return depthStencil.get();
        }
    }
    else
    {
        if (type < renderTargets.count())
        {
            return renderTargets[type].get();
        }
    }

    return nullptr;
}
