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

void Timebox::end()
{
    endTimestamp = getMicroseconds();
}
