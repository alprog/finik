export module render_surface;

import core;
import dx;

import gpu_resource;
import descriptor_handle;
import msaa;
export import surface_resolution;

export class RenderSurface
{
public:
    virtual void resize(SurfaceResolution resolution) = 0;

    GpuResource resource;
    DescriptorHandle handle;
    DescriptorHandle textureHandle;
};