export module frame_buffer;

import core;
import descriptor_handle;
import dx;
import command_list;
import gpu_resource;
import render_surface;
import mrt;

export class FrameBuffer
{
public:
    FrameBuffer(MRT mrt, IntSize resolution);
    void createHandles();

    void resize(IntSize resolution);
    void recreateRenderTarget();
    void recreateDepthStencil();

    void startRendering(CommandList& commandList);
    void endRendering(CommandList& commandList);

    IntSize resolution;

    bool renderTargetEnabled;
    RenderTarget renderTarget;
    
    bool depthStencilEnabled;
    DepthStencil depthStencil;

    D3D12_VIEWPORT viewport = {0};
    D3D12_RECT scissorRect;
};