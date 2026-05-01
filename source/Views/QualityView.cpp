module QualityView;

import Imgui;
import QualityManager;

QualityView::QualityView(const char* name)
    : View{name}
{
    loadCurrent();
}

void QualityView::update(float deltaTime)
{
    if (needApply)
    {
        QualityManager::GetInstance().apply(settings);
        needApply = false;
    }
}

void QualityView::loadCurrent()
{
    settings = QualityManager::GetInstance().getCurrent();
}

void QualityView::draw_content()
{
    static const char* msaa_items[]{"Off", "MSAAx2", "MSAAx4", "MSAAx8"};
    ImGui::Combo("Surface", reinterpret_cast<int*>(&settings.msaa), msaa_items, 4);

    ImGui::Checkbox("TAA", &settings.taa);
    ImGui::Checkbox("ShadowSnapping", &settings.shadowSnapping);
    ImGui::InputInt("ShadowMapSize", &settings.shadowMapResolution);
    static const char* presicion_items[]{"8 Bit", "16 Bit", "32 Bit"};
    ImGui::Combo("ShadowMapBits", reinterpret_cast<int*>(&settings.shadowMapPrecision), presicion_items, 3);

    ImGui::InputInt("PCFSize", &settings.pcfSize);

    bool IsSame = QualityManager::GetInstance().getCurrent() == settings;

    ImGui::BeginDisabled(IsSame);
    if (ImGui::Button("Apply"))
    {
        needApply = true;
    }
    if (ImGui::Button("Restore"))
    {
        loadCurrent();
    }
    ImGui::EndDisabled();
}
