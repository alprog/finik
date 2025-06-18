export module depth_stencil;

import render_surface;

export class DepthStencil : public RenderSurface
{
public:
    DepthStencil(SurfaceResolution resolution);

    void resize(SurfaceResolution siresolutionze) override;
};