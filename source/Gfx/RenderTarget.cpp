module;
#include "dx.h"
module RenderTarget;

import RenderSystem;

RenderTarget::RenderTarget(TextureFormat format, SurfaceResolution resolution)
    : RenderSurface{format}
{
    auto& engine = Single::Get<RenderSystem>().engine;
    handle = engine.getRtvHeap()->getNextHandle();
    textureHandle = engine.getCommonHeap()->getNextHandle();
    resize(resolution);
}

void RenderTarget::resize(SurfaceResolution resolution)
{
    auto& engine = Single::Get<RenderSystem>().engine;
    CD3DX12_RESOURCE_DESC resourceDesc(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D, 0,
        static_cast<uint32>(resolution.width),
        static_cast<uint32>(resolution.height),
        1, 1, format, resolution.sampleCount, 0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

    D3D12_CLEAR_VALUE clearValue;
    clearValue.Format = format;

    resource.reinit(resourceDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue);

    engine.getDevice()->CreateRenderTargetView(resource.getInternal(), nullptr, handle.getCPU());
    engine.getDevice()->CreateShaderResourceView(resource.getInternal(), nullptr, textureHandle.getCPU());
}

void RenderTarget::changeFormat(TextureFormat format)
{
    this->format = format;
    resize(getResolution());
}
