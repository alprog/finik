export module render_surface;

import dx;

import gpu_resource;
import descriptor_handle;

export class RenderSurface
{
public:
    GpuResource resource;
    DescriptorHandle handle;
    DescriptorHandle textureHandle;
};