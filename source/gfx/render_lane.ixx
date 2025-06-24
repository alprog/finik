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
    FrameBuffer& getHistoryBuffer();
    FrameBuffer& getResolveBuffer();
    FrameBuffer& getDebugBuffer();

    void render();

private:
    Scene& scene;
    Camera& camera;
    Matrix prevViewProjection;
    Vector2 prevJitter;
    SurfaceResolution resolution;

    GBuffer gBuffer;
    FrameBuffer lightBuffer;
    
    // TAA
    FrameBuffer historyBuffer;
    FrameBuffer resolvedBuffer;
    FrameBuffer debugBuffer;


    uint64 fenceValue = 0;

    Mesh* fullscreenQuad;
};
