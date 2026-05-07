export module QualityView;

import Finik.Core;
import View;
import QualitySettings;

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
    bool autoApply = false;
};
