export module depth_stencil;

import render_surface;

export class DepthStencil : public RenderSurface
{
public:
    DepthStencil(IntSize resolution);

    void resize(IntSize resolution) override;
};