export module DepthStencil;

import DX;
import RenderSurface;

export class DepthStencil : public RenderSurface
{
public:
    DepthStencil(SurfaceResolution resolution);

    void resize(SurfaceResolution resolution) override;
};
