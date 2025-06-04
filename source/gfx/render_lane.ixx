export module render_lane;

import core;
import frame_buffer;
import scene_system_fwd;
import render_pass;

export class RenderLane
{
public:
    RenderLane(Scene& scene, RenderPass pass, Camera& camera, IntSize resolution);
    void resize(IntSize resolution);

    FrameBuffer& getFrameBuffer();
    void render();

private:
    Scene& scene;
    Camera& camera;
    FrameBuffer frameBuffer;
    RenderPass pass;

    uint64 fenceValue = 0;
};