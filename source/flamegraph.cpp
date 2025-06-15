module flamegraph;

import imgui;

namespace finik
{
void drawFlamegraph(RingBuffer<profiler::Timebox>& timeboxes, int startIndex, int endIndex, uint64 startTime, uint64 endTime, Vector2 size)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        const uint64 timeInterval = endTime - startTime;

        const float widthPerTime = size.x / timeInterval;
        const float levelHeight = 20;

        const ImVec2 cursorPos = window->DC.CursorPos;
        const ImU32 fillColor = ImGui::GetColorU32(ImGuiCol_PlotHistogram);
        const ImU32 borderColor = ImGui::GetColorU32(ImGuiCol_PlotLines);

        for (int i = startIndex; i < endIndex; i++)
        {
            auto& timebox = timeboxes[i];

            ImVec2 startPos = cursorPos;
            ImVec2 endPos = cursorPos;

            startPos.x += widthPerTime * (timebox.startTimestamp - startTime);
            startPos.y += timebox.level * levelHeight;

            endPos.x += widthPerTime * (timebox.endTimestamp - startTime);
            endPos.y += (timebox.level + 1) * levelHeight;

            window->DrawList->AddRectFilled(startPos, endPos, fillColor, 0, ImDrawFlags_None);
            window->DrawList->AddRect(startPos, endPos, borderColor, 0, ImDrawFlags_None);
        }
    }
}
