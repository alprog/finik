module;
#include "dx.h"
module render_target;

import render_system;

RenderTarget::RenderTarget(SurfaceResolution resolution)
{
    RenderSystem& render_system = Single::Get<RenderSystem>();
    handle = render_system.getRtvHeap()->getNextHandle();
    textureHandle = render_system.getCommonHeap()->getNextHandle();
    resize(resolution);
}

void RenderTarget::resize(SurfaceResolution resolution)
{
    RenderSystem& render_system = Single::Get<RenderSystem>();
    CD3DX12_RESOURCE_DESC resourceDesc(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D, 0,
        static_cast<uint32>(resolution.width),
        static_cast<uint32>(resolution.height),
        1, 1, DXGI_FORMAT_R8G8B8A8_UNORM, resolution.sampleCount, 0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

    D3D12_CLEAR_VALUE clearValue;
    clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    resource.reinit(resourceDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue);

    render_system.get_device()->CreateRenderTargetView(resource.getInternal(), nullptr, handle.getCPU());
    render_system.get_device()->CreateShaderResourceView(resource.getInternal(), nullptr, textureHandle.getCPU());
}