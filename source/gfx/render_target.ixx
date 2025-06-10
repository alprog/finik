export module render_target;

import dx;
import render_surface;

export class RenderTarget : public RenderSurface
{
public:
    RenderTarget(SurfaceSize size);
    void resize(SurfaceSize size) override;
};