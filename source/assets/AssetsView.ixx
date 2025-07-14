export module AssetsView;

import View;

export class AssetsView : public View
{
public:
    AssetsView(const char* name);

protected:
    void draw_content() override;
};
