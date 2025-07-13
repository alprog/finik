export module shaders_view;

import view;
import imgui;
import shader_manager;

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