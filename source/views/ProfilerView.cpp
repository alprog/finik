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

    auto& cpuTimeboxes = profiler.GetCpuLane().timeboxes;

    int32 start = 0;
    int32 end = 0;

    int32 frameCount = 10;

    for (int32 i = cpuTimeboxes.count() - 1; i >= 0; i--)
    {
        if (!strcmp(cpuTimeboxes[i].label, "frame"))
        {
            if (end == 0)
            {
                end = i;
            }
            else
            {
                if (!frameCount--)
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

    auto& gpuTimeboxes = profiler.GetGpuLane().timeboxes;

    if (gpuTimeboxes.empty())
        return;

    end = gpuTimeboxes.count() - 1;

    for (start = end - 1; start > 0; start--)
    {
        if (gpuTimeboxes[start].startTimestamp < startTime)
            break;
    }

    finik::drawFlamegraph(gpuTimeboxes, start, end, startTime, endTime, Vector2(900, 30));
}
