export module frame_buffer;

import core;
import descriptor_handle;
import dx;
import command_list;
import gpu_resource;
import render_surface;
import render_target;
import depth_stencil;
import mrt;
import msaa;

// for intellisense

export class FrameBuffer
{
public:
    FrameBuffer(SurfaceResolution resolution, int32 renderTargetCount, bool depthBufferEnabled);

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