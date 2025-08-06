export module CameraController;

import Finik.Core;
import Camera;

export class CameraController
{
public:
    CameraController(Camera& camera);

    float GetVisibleAreaLength() const;
    float GetFieldOfView() const;
    float GetAngle() const;

    void HandleInput(float deltaTime);

    void RefreshCameraPosition();
    float CalculateDistance();

    Vector3 FocusPosition = Vector3::Zero;

    bool IsOrthogonal = false;
    float Rotation = 0;

    static constexpr float MinCellCount = 10.0f;
    static constexpr float MaxCellCount = 160.0f;
    float ZoomK = 0.5f;

    int ZoomStepCount = 20;

    static constexpr float MinFov = std::numbers::pi / 30.0f;
    static constexpr float MaxFov = std::numbers::pi / 3.0f;
    float FovK = 0.16f;

    static constexpr float MinAngle = std::numbers::pi * 0.200f;
    static constexpr float MaxAngle = std::numbers::pi * 0.499f;
    float AngleK = 0.5f;

    Camera& Camera;
};
