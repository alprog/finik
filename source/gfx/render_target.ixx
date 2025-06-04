export module render_target;

import dx;

import gpu_resource;

export class RenderTarget
{
public:
    GpuResource resource;
    D3D12_CPU_DESCRIPTOR_HANDLE handle;
};