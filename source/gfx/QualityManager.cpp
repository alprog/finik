module quality_manager;

import effect_manager;

void QualityManager::apply(QualitySettings settings)
{
    if (this->settings.msaa != settings.msaa)
    {
        for (auto& [name, effect] : EffectManager::GetInstance().Effects)
        {
            if (effect->getPipelineType() == PipelineType::Geometry)
            {
                effect->resetPso();
            }
        }
    }
    this->settings = settings;
}