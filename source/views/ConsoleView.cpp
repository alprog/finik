module ConsoleView;

import Imgui;

ConsoleView::ConsoleView(const char* name)
    : View{ name }
{
}

void ConsoleView::draw_content()
{
    ImGuiKeyChord Chord = ImGuiKey_C;
    if (ImGui::Button("Refresh") || ImGui::Shortcut(Chord))
    {
        int i = 0;
        i++;
    }
}
