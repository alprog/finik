module QualityManager;

import EffectManager;
import App;
import SurfaceResolution;
import Scene;
import RenderSystem;
import PipelineSettings;

void resetPsoForPipelineType(PipelineType type)
{
    for (auto& [name, effect] : EffectManager::GetInstance().getEffects())
    {
        if (effect->getPipelineType() == type)
        {
            effect->resetPso();
        }
    }
}

void QualityManager::apply(QualitySettings settings)
{
    if (this->settings.msaa != settings.msaa)
    {
        resetPsoForPipelineType(PipelineType::Geometry);
    }

    if (this->settings.shadowMapResolution != settings.shadowMapResolution)
    {
        auto& engine = Single::Get<RenderSystem>().engine;
        engine.get_command_queue().Flush();

        SurfaceResolution surfaceResolution = {settings.shadowMapResolution, settings.shadowMapResolution, 1};
        for (Scene* scene : App::GetInstance().scene_manager.scenes)
        {
            scene->light.shadowMap->resize(surfaceResolution);
            scene->light.pingPongBuffer->resize(surfaceResolution);
        }
    }

    if (this->settings.shadowMapPrecision != settings.shadowMapPrecision)
    {
        auto& engine = Single::Get<RenderSystem>().engine;
        engine.get_command_queue().Flush();

        SurfaceResolution surfaceResolution = {settings.shadowMapResolution, settings.shadowMapResolution, 1};
        auto format = settings.getShadowMapFormat();
        for (Scene* scene : App::GetInstance().scene_manager.scenes)
        {
            scene->light.shadowMap->renderTargets[0]->changeFormat(format);
            scene->light.pingPongBuffer->renderTargets[0]->changeFormat(format);
        }

        resetPsoForPipelineType(PipelineType::Shadow);
        resetPsoForPipelineType(PipelineType::ShadowPost);
    }

    this->settings = settings;
}
