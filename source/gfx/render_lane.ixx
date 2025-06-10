export module render_lane;

import core;
import frame_buffer;
import scene_system_fwd;
import render_pass;
import surface_size;
export import msaa;

export class SceneRenderLane
{
public:
    SceneRenderLane(Scene& scene, Camera& camera, SurfaceSize size);
    void resize(SurfaceSize size);

    FrameBuffer& getGBuffer();
    FrameBuffer& getLightBuffer();

    void render();

private:
    Scene& scene;
    Camera& camera;
    SurfaceSize size;

    FrameBuffer gBuffer;
    FrameBuffer lightBuffer;

    uint64 fenceValue = 0;
};
