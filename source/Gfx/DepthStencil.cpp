module;
#include "dx.h"
module DepthStencil;

import RenderSystem;

DepthStencil::DepthStencil(SurfaceResolution resolution)
    : RenderSurface{TextureFormat::DXGI_FORMAT_D24_UNORM_S8_UINT}
{
    auto& engine = Single::Get<RenderSystem>().engine;
    handle = engine.getDsvHeap()->getNextHandle();
    textureHandle = engine.getCommonHeap()->getNextHandle();
    resize(resolution);
}

void DepthStencil::resize(SurfaceResolution resolution)
{
    CD3DX12_RESOURCE_DESC resourceDesc(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D, 0,
        static_cast<uint32>(resolution.width),
        static_cast<uint32>(resolution.height),
        1, 1, format, resolution.sampleCount, 0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
 
    D3D12_CLEAR_VALUE clearValue;
    clearValue.Format = format;
    clearValue.DepthStencil.Depth = 1.0f;
    clearValue.DepthStencil.Stencil = 0;

    auto& engine = Single::Get<RenderSystem>().engine;

    reinitInternalResource(resourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue);

    engine.getDevice()->CreateDepthStencilView(internalResource, nullptr, handle.getCPU());

    // Create the shader resource view
    D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    SRVDesc.ViewDimension = resolution.sampleCount > 1 ? D3D12_SRV_DIMENSION_TEXTURE2DMS : D3D12_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;    

    SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    engine.getDevice()->CreateShaderResourceView(internalResource, &SRVDesc, textureHandle.getCPU());
}
