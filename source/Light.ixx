export module Light;

import Finik.Core;
import FrameBuffer;
import Camera;

export struct Light
{
    Vector4 direction;
    Camera shadowCamera;
    std::unique_ptr<FrameBuffer> shadowMap;
};
