export module Lane;

import Finik.Core;
import Timebox;

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
