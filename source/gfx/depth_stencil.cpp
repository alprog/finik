module;
#include "dx.h"
module depth_stencil;

import render_system;

DepthStencil::DepthStencil(SurfaceResolution resolution)
{
    RenderSystem& render_system = Single::Get<RenderSystem>();
    handle = render_system.getDsvHeap()->getNextHandle();
    textureHandle = render_system.getCommonHeap()->getNextHandle();
    resize(resolution);
}

void DepthStencil::resize(SurfaceResolution resolution)
{
    CD3DX12_RESOURCE_DESC resourceDesc(
        D3D12_RESOURCE_DIMENSION_TEXTURE2D, 0,
        static_cast<uint32>(resolution.width),
        static_cast<uint32>(resolution.height),
        1, 1, DXGI_FORMAT_D32_FLOAT, resolution.sampleCount, 0,
        D3D12_TEXTURE_LAYOUT_UNKNOWN,
        D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
 

    D3D12_CLEAR_VALUE clearValue;
    clearValue.Format = DXGI_FORMAT_D32_FLOAT;
    clearValue.DepthStencil.Depth = 1.0f;
    clearValue.DepthStencil.Stencil = 0;

    RenderSystem& render_system = Single::Get<RenderSystem>();

    resource.reinit(resourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue);

    render_system.get_device()->CreateDepthStencilView(resource.getInternal(), nullptr, handle.getCPU());

    // Create the shader resource view
    D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};
    SRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
    SRVDesc.ViewDimension = resolution.sampleCount > 1 ? D3D12_SRV_DIMENSION_TEXTURE2DMS : D3D12_SRV_DIMENSION_TEXTURE2D;
    SRVDesc.Texture2D.MipLevels = 1;    

    SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    render_system.get_device()->CreateShaderResourceView(resource.getInternal(), &SRVDesc, textureHandle.getCPU());
}