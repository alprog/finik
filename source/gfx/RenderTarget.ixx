export module render_target;

import dx;
import render_surface;

export class RenderTarget : public RenderSurface
{
public:
    RenderTarget(TextureFormat format, SurfaceResolution resolution);
    void resize(SurfaceResolution resolution) override;
};