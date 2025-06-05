export module render_surface;

import core;
import dx;

import gpu_resource;
import descriptor_handle;

export class RenderSurface
{
public:
    GpuResource resource;
    DescriptorHandle handle;
    DescriptorHandle textureHandle;

    virtual void resize(IntSize resolution) = 0;
};

export class RenderTarget : public RenderSurface
{
public:
    RenderTarget(IntSize resolution);
    void resize(IntSize resolution) override;
};

export class DepthStencil : public RenderSurface
{
public:
    DepthStencil(IntSize resolution);

    void resize(IntSize resolution) override;
};