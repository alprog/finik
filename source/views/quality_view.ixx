export module quality_view;

import core;
import view;
import quality_settings;

// for intellisense

export class QualityView : public View
{
public:
    QualityView(const char* name);
    
    void update(float deltaTime) override;
    
protected:
    void loadCurrent();
    void draw_content() override;

    QualitySettings settings;
    bool needApply = false;
    int selectedMsaaType = 0;
};