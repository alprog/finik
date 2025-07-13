export module render_surface;

import Finik.Core;
import dx;

import gpu_resource;
import descriptor_handle;
import msaa;
export import surface_resolution;

export class RenderSurface
{
protected:
    RenderSurface(TextureFormat format)
        : format{format}
    {
    }

public:
    virtual void resize(SurfaceResolution resolution) = 0;

    TextureFormat format;
    GpuResource resource;
    DescriptorHandle handle;
    DescriptorHandle textureHandle;
};