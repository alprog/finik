export module depth_stencil;

import render_surface;

export class DepthStencil : public RenderSurface
{
public:
    DepthStencil(SurfaceSize size);

    void resize(SurfaceSize size) override;
};