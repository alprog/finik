export module ProfilerView;

import View;

export class ProfilerView : public View
{
    using View::View;

public:
    void draw_content() override;
};
