export module FrameBuffer;

import Finik.Core;
import DescriptorHandle;
import DX;
import CommandList;
import GpuResource;
import RenderSurface;
import RenderTarget;
import DepthStencil;
import Mrt;
import Msaa;

// for intellisense

export class FrameBuffer
{
public:
    FrameBuffer(SurfaceResolution resolution, Array<TextureFormat> formats, bool depthBufferEnabled);

    void resize(SurfaceResolution resolution);

    void startRendering(CommandList& commandList);
    void endRendering(CommandList& commandList);

    RenderSurface* getRenderSurface(MRT type) const;

    SurfaceResolution resolution;

    Array<std::unique_ptr<RenderTarget>> renderTargets;
    std::unique_ptr<DepthStencil> depthStencil;

    D3D12_VIEWPORT viewport = {0};
    D3D12_RECT scissorRect;
};
