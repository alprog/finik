export module ProfilerView;

import View;
import Finik.Core;
import Timebox;

export class ProfilerView : public View
{
    using Timeboxes = HistoryRingBuffer<finik::profiler::Timebox>;

    using View::View;

public:    
    void draw_content() override;
    void drawTimeboxes(Timeboxes& cpuTimeboxes, Timeboxes gpuTimeboxes);

private:
    bool paused = false;

    Timeboxes cpuTimeboxes = Timeboxes(0);
    Timeboxes gpuTimeboxes = Timeboxes(0);
    int32 frameCount = 10;
};
