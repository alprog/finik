module quality_view;

import imgui;
import quality_manager;

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
    selectedMsaaType = static_cast<int32>(settings.msaa);
}

void QualityView::draw_content()
{
    static const char* items[]{"Off", "MSAAx2", "MSAAx4", "MSAAx8"};
    ImGui::Combo("Surface", &selectedMsaaType, items, 4);
    settings.msaa = static_cast<MSAA>(selectedMsaaType);

    ImGui::Checkbox("TAA", &settings.taa);

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