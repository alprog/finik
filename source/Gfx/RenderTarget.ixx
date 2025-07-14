export module RenderTarget;

import DX;
import RenderSurface;

export class RenderTarget : public RenderSurface
{
public:
    RenderTarget(TextureFormat format, SurfaceResolution resolution);
    void resize(SurfaceResolution resolution) override;
};
