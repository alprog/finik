export module Timebox;

import Finik.Core;

export namespace finik::profiler
{
    struct Timebox
    {
        Timebox(const char* label, uint8 level);
        Timebox(const char* label, uint8 level, uint64 startTimestamp, uint64 endTimestamp);
        void end();

        const char* label;
        uint64 startTimestamp;
        uint64 endTimestamp;
        uint8 level;
    };

    static_assert(sizeof(Timebox) == 32);
}
