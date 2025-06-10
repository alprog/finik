export module render_surface;

import core;
import dx;

import gpu_resource;
import descriptor_handle;
import msaa;
export import surface_size;

export class RenderSurface
{
public:
    GpuResource resource;
    DescriptorHandle handle;
    DescriptorHandle textureHandle;

    virtual void resize(SurfaceSize size) = 0;
};