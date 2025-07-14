export module TextureView;

import View;
import Scene;

export class TextureView : public View
{
public:
    TextureView(const char* name, Scene& scene);

protected:
    void draw_content() override;

    Scene& scene;
};
