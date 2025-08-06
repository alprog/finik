module CameraController;

import Finik.Core;
import Imgui;

CameraController::CameraController(::Camera& camera)
    : Camera{camera}
{
    RefreshCameraPosition();
}

float CameraController::GetVisibleAreaLength() const
{
    return lerp(MaxCellCount, MinCellCount, std::sqrt(ZoomK));
}

float CameraController::GetFieldOfView() const
{
    return IsOrthogonal ? 0 : lerp(MinFov, MaxFov, FovK);
}

float CameraController::GetAngle() const
{
    return lerp(MinAngle, MaxAngle, AngleK);
}

void CameraController::HandleInput(float deltaTime)
{
    auto moveDirection = Vector3::Zero;

    if (ImGui::IsKeyDown(ImGuiKey_W))
    {
        moveDirection += Vector3::Forward;
    }
    if (ImGui::IsKeyDown(ImGuiKey_S))
    {
        moveDirection += Vector3::Backward;
    }
    if (ImGui::IsKeyDown(ImGuiKey_A))
    {
        moveDirection += Vector3::Left;
    }
    if (ImGui::IsKeyDown(ImGuiKey_D))
    {
        moveDirection += Vector3::Right;
    }

    moveDirection = Matrix::RotationZ(Rotation).MultiplyDirection(moveDirection);

    ZoomK += ImGui::GetIO().MouseWheel / ZoomStepCount;
    if (ImGui::IsKeyDown(ImGuiKey_Q))
    {
        ZoomK -= deltaTime / 2;
    }
    if (ImGui::IsKeyDown(ImGuiKey_E))
    {
        ZoomK += deltaTime / 2;
    }
    ZoomK = std::clamp(ZoomK, 0.0f, 1.0f);

    float PanningScreenPerSecond = 1.0f;
    if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
    {
        PanningScreenPerSecond *= 3;
    }
    FocusPosition += moveDirection * GetVisibleAreaLength() * PanningScreenPerSecond * deltaTime;

    if (ImGui::IsKeyDown(ImGuiKey_Z))
    {
        AngleK += deltaTime / 2;
    }
    if (ImGui::IsKeyDown(ImGuiKey_X))
    {
        AngleK -= deltaTime / 2;
    }
    AngleK = std::clamp(AngleK, 0.0f, 1.0f);

    if (ImGui::IsKeyDown(ImGuiKey_1))
    {
        IsOrthogonal = false;
    }
    if (ImGui::IsKeyDown(ImGuiKey_2))
    {
        IsOrthogonal = true;
    }

    if (ImGui::IsKeyDown(ImGuiKey_O))
    {
        FovK -= deltaTime / 2;
        IsOrthogonal = FovK <= 0;
    }
    if (ImGui::IsKeyDown(ImGuiKey_P))
    {
        IsOrthogonal = false;
        FovK += deltaTime / 2;
    }
    FovK = std::clamp(FovK, 0.0f, 1.0f);

    if (ImGui::IsKeyDown(ImGuiKey_Y))
    {
        Rotation -= std::numbers::pi * deltaTime / 4;
    }
    if (ImGui::IsKeyDown(ImGuiKey_U))
    {
        Rotation += std::numbers::pi * deltaTime / 4;
    }
}

void CameraController::RefreshCameraPosition()
{
    auto lookDirection = Matrix::RotationZ(Rotation).MultiplyDirection(Vector3::Forward);

    auto angle = GetAngle();
    auto x = -lookDirection.x * std::cos(angle);
    auto y = -lookDirection.y * std::cos(angle);
    auto z = std::sin(angle);
    auto OffsetDirection = Vector3(x, y, z);


    auto Distance = CalculateDistance();
    Camera.position = FocusPosition + OffsetDirection * Distance;
    Camera.lookAt = FocusPosition;

    Camera.FieldOfView = GetFieldOfView();
    Camera.OrthoSize = GetVisibleAreaLength() * std::sin(angle);

    Camera.calcViewMatrix();
    Camera.calcProjectionMatrix();
}

float CameraController::CalculateDistance()
{
    auto fieldOfView = GetFieldOfView();
    if (fieldOfView == 0)
    {
        return 80.0f;
    }

    auto viewAngle = GetAngle();

    auto nearAngle = viewAngle + fieldOfView / 2;
    auto farAngle = viewAngle - fieldOfView / 2;

    auto nearXperY = std::cos(nearAngle) / std::sin(nearAngle);
    auto farXperY = std::cos(farAngle) / std::sin(farAngle);

    // (farXperY - nearXperY) * Y = VisibleAreaLength
    auto y = GetVisibleAreaLength() / (farXperY - nearXperY);
    return y / std::sin(viewAngle);
}
