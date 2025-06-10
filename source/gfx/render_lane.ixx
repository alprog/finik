export module render_lane;

import core;
import frame_buffer;
import scene_system_fwd;
import render_pass;
export import msaa;

export class RenderLane
{
public:
    RenderLane(Scene& scene, RenderPass pass, Camera& camera, IntSize resolution, MSAA msaa);
    void resize(IntSize resolution, MSAA msaa);

    FrameBuffer& getFrameBuffer();
    void render();

private:
    Scene& scene;
    Camera& camera;
    FrameBuffer frameBuffer;
    RenderPass pass;

    uint64 fenceValue = 0;
};

export class SceneRenderLane
{
    SceneRenderLane(Scene& scene, Camera& camera, IntSize resolution, MSAA msaa);
    void resize(IntSize resolution, MSAA msaa);

    FrameBuffer& getGBuffer();
    FrameBuffer& getLightBuffer();

    void render();

private:
    Scene& scene;
    Camera& camera;

    FrameBuffer gBuffer;
    FrameBuffer lightBuffer;

    uint64 fenceValue = 0;
};
