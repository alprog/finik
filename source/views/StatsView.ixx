export module stats_view;

import view;
import scene_view;

export class StatsView : public View
{
public:
    StatsView(const char* name, SceneView& sceneView);

protected:
    void draw_content() override;
    
    SceneView& sceneView;
};