module View;

import Imgui;

View::View(const char* name)
    : name{name}
{
}

void View::draw()
{
    bool open = true;
    ImGui::Begin(name, &open, ImGuiWindowFlags_NoCollapse);// | ImGuiWindowFlags_NoBringToFrontOnFocus);
    draw_content();
    ImGui::End();
}
