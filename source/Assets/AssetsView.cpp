module AssetsView;

import Finik.Core;
import Imgui;
import Assets;

AssetsView::AssetsView(const char* name)
    : View{ name }
{
}

void AssetsView::draw_content()
{
    auto& assets = Assets::GetInstance();

    ImGui::Text("Loaded:");
    for (auto& [path, desc] : assets.asset_descs)
    {
        if (desc.is_loaded())
        {
            ImGui::Text("%s (%i)", path.c_str(), desc.loaded_asset.use_count());
        }
    }
}
