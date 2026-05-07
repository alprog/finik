export module Flamegraph;

import Finik.Core;
import Timebox;

export namespace finik
{
    void drawFlamegraph(HistoryRingBuffer<profiler::Timebox>& timeboxes, int startIndex, int endIndex, uint64 startTime, uint64 endTime, Vector2 size);
}
