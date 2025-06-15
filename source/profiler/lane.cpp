module lane;

import timer;

using namespace finik::profiler;

Lane::Lane()
    : level { 0 }
    , timeboxes{ 1000 }
{
}

Timebox& Lane::startTimebox(const char* label)
{
    return timeboxes.emplace_next(label, level++);
}

void Lane::endTimebox(Timebox& timebox)
{
    timebox.end();
    level--;
}