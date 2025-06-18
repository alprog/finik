export module render_lane;

import core;
import frame_buffer;
import g_buffer;
import scene_system_fwd;
import render_pass;
import surface_resolution;
import mesh;
export import msaa;

// for intellisense

export class SceneRenderLane
{
public:
    SceneRenderLane(Scene& scene, Camera& camera, SurfaceResolution resolution);
    void resize(SurfaceResolution resolution);

    FrameBuffer& getGBuffer();
    FrameBuffer& getLightBuffer();

    void render();

private:
    Scene& scene;
    Camera& camera;
    SurfaceResolution resolution;

    GBuffer gBuffer;
    FrameBuffer lightBuffer;

    uint64 fenceValue = 0;

    Mesh* fullscreenQuad;
};
