export module render_surface;

import dx;

import gpu_resource;
import descriptor_handle;

export class RenderSurface
{
public:
    GpuResource resource;
    D3D12_CPU_DESCRIPTOR_HANDLE handle;
};