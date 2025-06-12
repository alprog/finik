export module light;

import core;
import frame_buffer;
import camera;

export struct Light
{
    Vector4 direction;
    Camera shadowCamera;
    std::unique_ptr<FrameBuffer> shadowMap;
};