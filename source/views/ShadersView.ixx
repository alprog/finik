export module ShadersView;

import View;
import Imgui;
import ShaderManager;

export class ShadersView : public View
{
    using View::View;

public:
    void draw_content() override
    {
        auto& shaders = ShaderManager::GetInstance().getAllShaders();

        for (auto& [key, shader] : shaders)
        {
            ImGui::Text("%s", key.AssetPath.c_str());
            ImGui::Text("%s", shader->errorMessage.c_str());
        }
    }
};
