export module shadow_maps;

import core;
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