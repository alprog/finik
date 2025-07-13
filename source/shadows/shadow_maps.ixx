export module shadow_maps;

import Finik.Core;
import render_lane;
import camera;

export class Scene;

export class ShadowMaps
{
public:
    ShadowMaps(Scene& scene)
        : scene{scene}
        , lightCamera{}
    {
    }

private:
    Scene& scene;

    Camera lightCamera;
};