module QualityManager;

import EffectManager;
import App;
import SurfaceResolution;
import Scene;
import RenderSystem;

void QualityManager::apply(QualitySettings settings)
{
    if (this->settings.msaa != settings.msaa)
    {
        for (auto& [name, effect] : EffectManager::GetInstance().getEffects())
        {
            if (effect->getPipelineType() == PipelineType::Geometry)
            {
                effect->resetPso();
            }
        }
    }

    if (this->settings.shadowMapResolution != settings.shadowMapResolution)
    {
        auto& engine = Single::Get<RenderSystem>().engine;
        engine.get_command_queue().Flush();

        SurfaceResolution surfaceResolution = {settings.shadowMapResolution, settings.shadowMapResolution, 1};
        for (Scene* scene : App::GetInstance().scene_manager.scenes)
        {
            scene->light.shadowMap->resize(surfaceResolution);
        }
    }

    this->settings = settings;
}
