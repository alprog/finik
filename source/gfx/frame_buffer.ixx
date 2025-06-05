export module frame_buffer;

import core;
import descriptor_handle;
import dx;
import command_list;
import gpu_resource;
import render_surface;

export class FrameBuffer
{
public:
    FrameBuffer(IntSize resolution, int32 renderTargetCount, bool depthBufferEnabled);

    void resize(IntSize resolution);

    void startRendering(CommandList& commandList);
    void endRendering(CommandList& commandList);

    IntSize resolution;

    Array<std::unique_ptr<RenderTarget>> renderTargets;
    std::unique_ptr<DepthStencil> depthStencil;

    D3D12_VIEWPORT viewport = {0};
    D3D12_RECT scissorRect;
};