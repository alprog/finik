export module RenderSurface;

import Finik.Core;
import DX;

import GpuResource;
import DescriptorHandle;
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
