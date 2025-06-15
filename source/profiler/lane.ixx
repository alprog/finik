export module lane;

import core;
import timebox;

export namespace finik::profiler
{
    class Lane
    {
    public:
        Lane();

        RingBuffer<Timebox> timeboxes;
        int level;

        Timebox& startTimebox(const char* label);
        void endTimebox(Timebox& timebox);
    };
}
