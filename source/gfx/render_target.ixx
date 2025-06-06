export module render_target;

import dx;
import render_surface;

export class RenderTarget : public RenderSurface
{
public:
    RenderTarget(IntSize resolution, MSAA msaa);
    void resize(IntSize resolution, MSAA msaa) override;
};