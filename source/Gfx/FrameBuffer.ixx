export module FrameBuffer;

import RenderEngine;
import GpuResource;
import RenderSurface;
import RenderTarget;
import DepthStencil;
import Mrt;
import Msaa;

export class FrameBuffer
{
public:
    FrameBuffer(SurfaceResolution resolution, Array<TextureFormat> formats, bool depthBufferEnabled);

    void resize(SurfaceResolution resolution);

    void startRendering(CommandList& commandList);
    void endRendering(CommandList& commandList);

    RenderSurface* getRenderSurface(MRT type) const;

    SurfaceResolution resolution;

    Array<UniquePtr<RenderTarget>> renderTargets;
    UniquePtr<DepthStencil> depthStencil;

    D3D12_VIEWPORT viewport = {0};
    D3D12_RECT scissorRect;
};
