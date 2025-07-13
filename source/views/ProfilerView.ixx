export module profiler_view;

import view;

export class ProfilerView : public View
{
    using View::View;

public:
    void draw_content() override;
};