module Timebox;

import Timer;

using namespace finik::profiler;

Timebox::Timebox(const char* label, uint8 level)
    : label{ label}
    , startTimestamp { getMicroseconds() }
    , endTimestamp { 0 }
    , level { level }
{
}

Timebox::Timebox(const char* label, uint8 level, uint64 startTimestamp, uint64 endTimestamp)
    : label{label}
    , level{level}
    , startTimestamp{startTimestamp}
    , endTimestamp{endTimestamp}
{
}

void Timebox::end()
{
    endTimestamp = getMicroseconds();
}
