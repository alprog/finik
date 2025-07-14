export module SceneView;

import Finik.Core;
import RenderLane;
import View;
import Camera;
import CameraController;
import Scene;

// for intellisense

export class SceneView : public View
{
public:
    SceneView(const char* name, Scene& scene);

    const CameraController& getCameraController() const;

    void update(float deltaTime) override;

protected:
    void draw_content() override;

public:
    Scene& scene;

protected:
    Camera camera;
    CameraController cameraContoller;

    std::shared_ptr<SceneRenderLane> renderLane;
};
