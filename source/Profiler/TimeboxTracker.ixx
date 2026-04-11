export module TimeboxTracker;

import Finik.Core;
import Timebox;

export namespace finik::profiler
{
    struct TimeboxTracker
    {
        explicit TimeboxTracker(const char* label);
        ~TimeboxTracker();

        Timebox& timebox;
    };
}

#define PROFILE(x) finik::profiler::TimeboxTracker timebox_tracker(x)

export using Profile = finik::profiler::TimeboxTracker;
