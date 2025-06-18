export module depth_stencil;

import dx;
import render_surface;

// for intellisense

export class DepthStencil : public RenderSurface
{
public:
    DepthStencil(SurfaceResolution resolution);

    void resize(SurfaceResolution siresolutionze) override;
};