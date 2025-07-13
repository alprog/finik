export module assets_view;

import view;

export class AssetsView : public View
{
public:
    AssetsView(const char* name);

protected:
    void draw_content() override;
};