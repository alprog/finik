export module texture_view;

import view;
import scene;

export class TextureView : public View
{
public:
    TextureView(const char* name, Scene& scene);

protected:
    void draw_content() override;

    Scene& scene;
};