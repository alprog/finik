module stats_view;

import imgui;
import scene;

StatsView::StatsView(const char* name, SceneView& sceneView)
    : View{ name }
    , sceneView{ sceneView }
{
}

void StatsView::draw_content()
{
    auto& controller = sceneView.getCameraController();


    constexpr float radToDegree = 180 / std::numbers::pi;

    ImGui::Text("focus: %f %f", controller.FocusPosition.x, controller.FocusPosition.y);
    ImGui::Text("rotation: %f", controller.Rotation * radToDegree);
    ImGui::Text("angle: %f", controller.GetAngle() * radToDegree);
    ImGui::Text("fov: %f", controller.GetFieldOfView() * radToDegree);
    ImGui::Text("vert.area: %f", controller.GetVisibleAreaLength());

    ImGui::Text("aspect: %f", controller.Camera.AspectRatio);

    ImGui::Text("casted: %f %f", sceneView.scene.castedPos.x, sceneView.scene.castedPos.y);
}