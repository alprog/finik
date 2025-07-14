export module DepthStencil;

import DX;
import RenderSurface;

// for intellisense

export class DepthStencil : public RenderSurface
{
public:
    DepthStencil(SurfaceResolution resolution);

    void resize(SurfaceResolution resolution) override;
};
