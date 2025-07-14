export module RenderLane;

import Finik.Core;
import FrameBuffer;
import GBuffer;
import SceneSystemFwd;
import RenderPass;
import SurfaceResolution;
import Mesh;
export import Msaa;

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
