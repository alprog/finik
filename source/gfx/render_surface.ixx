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