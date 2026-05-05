module Flamegraph;

import Imgui;

namespace finik
{
    void drawBoxInfo(profiler::Timebox& box)
    {
        ImGui::Text(box.label);

        float ms = static_cast<float>(box.endTimestamp - box.startTimestamp) / 1000;
        ImGui::Text("Time: %f ms", ms);

    }

    void drawFlamegraph(RingBuffer<profiler::Timebox>& timeboxes, int startIndex, int endIndex, uint64 startTime, uint64 endTime, Vector2 size)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        const uint64 timeInterval = endTime - startTime;

        const float widthPerTime = size.x / timeInterval;
        const float levelHeight = 20;

        const ImVec2 cursorPos = window->DC.CursorPos;
        const ImU32 plotColor = ImGui::GetColorU32(ImGuiCol_PlotHistogram);
        const ImU32 plotColorHovered = ImGui::GetColorU32(ImGuiCol_PlotHistogramHovered);
        const ImU32 borderColor = ImGui::GetColorU32(ImGuiCol_PlotLines);
        const ImU32 textColor = ImGui::GetColorU32(ImVec4(0, 0, 0, 1));

        profiler::Timebox* hoveredTimebox = nullptr;

        ImVec2 mousePos = ImGui::GetMousePos();
        for (int i = startIndex; i < endIndex; i++)
        {
            auto& timebox = timeboxes[i];

            ImVec2 startPos = cursorPos;
            ImVec2 endPos = cursorPos;

            startPos.x += widthPerTime * (timebox.startTimestamp - startTime);
            startPos.y += timebox.level * levelHeight;

            endPos.x += widthPerTime * (timebox.endTimestamp - startTime);
            endPos.y += (timebox.level + 1) * levelHeight;

            if (endPos.x - startPos.x < 1)
            {
                continue;
            }

            auto fillColor = plotColor;
            if (mousePos.x >= startPos.x && mousePos.x <= endPos.x &&
                mousePos.y >= startPos.y && mousePos.y <= endPos.y)
            {
                hoveredTimebox = &timebox;
                fillColor = plotColorHovered;
            }
           
            window->DrawList->AddRectFilled(startPos, endPos, fillColor, 0, ImDrawFlags_None);
            window->DrawList->AddRect(startPos, endPos, borderColor, 0, ImDrawFlags_None);

            window->DrawList->AddText(startPos, textColor, timebox.label);
        }
        
        if (hoveredTimebox)
        {
            ImGui::BeginTooltip();
            drawBoxInfo(*hoveredTimebox);
            ImGui::EndTooltip();
        }
    }
}
