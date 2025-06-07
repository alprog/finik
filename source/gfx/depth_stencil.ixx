export module depth_stencil;

import render_surface;

export class DepthStencil : public RenderSurface
{
public:
    DepthStencil(IntSize resolution, MSAA msaa);

    void resize(IntSize resolution, MSAA msaa) override;
};