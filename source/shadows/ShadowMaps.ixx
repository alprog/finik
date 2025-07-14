export module ShadowMaps;

import Finik.Core;
import RenderLane;
import Camera;

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
