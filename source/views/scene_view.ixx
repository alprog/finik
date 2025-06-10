export module scene_view;

import core;
import render_lane;
import view;
import camera;
import camera_controller;
import scene;

// for intellisense

export class SceneView : public View
{
public:
    SceneView(const char* name, Scene& scene);

    const CameraController& getCameraController() const;

    void update(float deltaTime) override;

protected:
    void draw_content() override;
    void setupShadowCamera();

public:
    Scene& scene;

protected:
    Camera shadowCamera;
    Camera camera;
    CameraController cameraContoller;

    std::shared_ptr<SceneRenderLane> renderLane;
};