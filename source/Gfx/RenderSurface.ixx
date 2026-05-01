export module RenderSurface;

import GfxDevice;
import GpuResource;
import GfxEnums;
export import SurfaceResolution;

export class RenderSurface : public GpuResource
{
protected:
    RenderSurface(TextureFormat format)
        : format{format}
    {
    }

public:
    SurfaceResolution getResolution() const
    {
        const auto& desc = getInternal()->GetDesc();
        return {
            static_cast<int32>(desc.Width),
            static_cast<int32>(desc.Height),
            static_cast<int32>(desc.SampleDesc.Count)
        };
    }
    
    virtual void resize(SurfaceResolution resolution) = 0;

    TextureFormat format;
    DescriptorHandle handle;
    DescriptorHandle textureHandle;
};
