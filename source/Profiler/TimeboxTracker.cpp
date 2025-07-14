module TimeboxTracker;

import Profiler;
import App;
import Timer;

using namespace finik::profiler;

TimeboxTracker::TimeboxTracker(const char* label)
    : timebox{ App::GetInstance().profiler.GetCpuLane().startTimebox(label) }
{
}

TimeboxTracker::~TimeboxTracker()
{
    App::GetInstance().profiler.GetCpuLane().endTimebox(timebox);
}
