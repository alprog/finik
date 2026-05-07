module ProfilerView;

import Finik.Core;
import App;
import Imgui;
import Flamegraph;

void ProfilerView::draw_content()
{
    auto& profiler = App::GetInstance().profiler;
    ImGui::Text("frame: %i", profiler.getFrameIndex());
    ImGui::Text("deltaTime: %f", profiler.getDeltaTime());
    ImGui::Text("FPS: %f", profiler.getFPS());

    ImGui::InputInt("frame count", &frameCount, 1, 30);
    ImGui::SameLine();
    if (ImGui::Checkbox("pause", &paused))
    {
        if (paused)
        {
            cpuTimeboxes = profiler.GetCpuLane().timeboxes;
            gpuTimeboxes = profiler.GetGpuLane().timeboxes;
        }
    }

    if (paused)
    {
        drawTimeboxes(cpuTimeboxes, gpuTimeboxes);
    }
    else
    {
        drawTimeboxes(profiler.GetCpuLane().timeboxes, profiler.GetGpuLane().timeboxes);
    }
}

void ProfilerView::drawTimeboxes(Timeboxes& cpuTimeboxes, Timeboxes gpuTimeboxes)
{    
    int32 start = 0;
    int32 end = 0;

    int32 frameCount = this->frameCount;

    int32 min = cpuTimeboxes.getMinIndex();
    int32 max = cpuTimeboxes.getMaxIndex();
    for (int32 i = max; i >= min; i--)
    {
        if (!strcmp(cpuTimeboxes[i].label, "frame"))
        {
            if (end == 0)
            {
                end = i;
            }
            else
            {
                if (!--frameCount)
                {
                    start = i;
                    break;
                }
            }
        }
    }

    if (end == 0)
        return;

    const uint64 startTime = cpuTimeboxes[start].startTimestamp;
    const uint64 endTime = cpuTimeboxes[end - 1].endTimestamp;

    finik::drawFlamegraph(cpuTimeboxes, start, end, startTime, endTime, Vector2(900, 30));


    if (gpuTimeboxes.empty())
        return;

    end = gpuTimeboxes.getMaxIndex();

    for (start = end - 1; start > 0; start--)
    {
        auto box = gpuTimeboxes[start];
        if (box.startTimestamp != 0 && box.startTimestamp < startTime)
            break;
    }

    finik::drawFlamegraph(gpuTimeboxes, start, end, startTime, endTime, Vector2(900, 30));
}