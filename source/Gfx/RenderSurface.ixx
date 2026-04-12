export module RenderSurface;

import GfxDevice;
import GpuResource;
import Msaa;
export import SurfaceResolution;

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
