export module quality_view;

import core;
import view;

// for intellisense

export class QualityView : public View
{
public:
    QualityView(const char* name);

    void update(float deltaTime) override;

protected:
    void draw_content() override;
};