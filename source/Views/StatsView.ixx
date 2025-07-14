export module StatsView;

import View;
import SceneView;

export class StatsView : public View
{
public:
    StatsView(const char* name, SceneView& sceneView);

protected:
    void draw_content() override;
    
    SceneView& sceneView;
};
