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
    void resize(IntSize resolution) override
    {

    }
};

export class DepthStencil : public RenderSurface
{
public:
    DepthStencil();

    void resize(IntSize resolution) override;
};