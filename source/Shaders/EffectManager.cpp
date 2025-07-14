module EffectManager;

import ShaderManager;
import AssetPath;

void EffectManager::init()
{
    auto& shaderManager = ShaderManager::GetInstance();

    {
        auto effect = std::make_shared<Effect>("standard");
        AssetPath path = "shaders/shadersTextured.hlsl";
        effect->setVertexShader(shaderManager.getVertexShader(path, "VSMain"));
        effect->setPixelShader(shaderManager.getPixelShader(path, "PSMain"));
        Effects[effect->name] = effect;
    }

    {
        auto effect = std::make_shared<Effect>("3dsprite");
        AssetPath path = "shaders/3dsprite.hlsl";
        effect->setVertexShader(shaderManager.getVertexShader(path, "VSMain"));
        effect->setPixelShader(shaderManager.getPixelShader(path, "PSMain"));
        Effects[effect->name] = effect;
    }

    {
        auto effect = std::make_shared<Effect>("grid");
        AssetPath path = "shaders/grid.hlsl";
        effect->setVertexShader(shaderManager.getVertexShader(path, "VSMain"));
        effect->setPixelShader(shaderManager.getPixelShader(path, "PSMain"));
        Effects[effect->name] = effect;
    }

    {
        auto effect = std::make_shared<Effect>("directional_light");
        AssetPath path = "shaders/directional_light.hlsl";
        effect->setPipelineType(PipelineType::Lighting);
        effect->setVertexShader(shaderManager.getVertexShader(path, "VSMain"));
        effect->setPixelShader(shaderManager.getPixelShader(path, "PSMain"));
        Effects[effect->name] = effect;
    }

    {
        auto effect = std::make_shared<Effect>("taa_resolve");
        AssetPath path = "shaders/taa_resolve.hlsl";
        effect->setPipelineType(PipelineType::ScreenSpace);
        effect->setVertexShader(shaderManager.getVertexShader(path, "VSMain"));
        effect->setPixelShader(shaderManager.getPixelShader(path, "PSMain"));
        Effects[effect->name] = effect;
    }

    {
        auto effect = std::make_shared<Effect>("taa_resolve_debug");
        AssetPath path = "shaders/taa_resolve.hlsl";
        effect->setPipelineType(PipelineType::ScreenSpace);
        effect->setVertexShader(shaderManager.getVertexShader(path, "VSMain"));
        effect->setPixelShader(shaderManager.getPixelShader(path, "PSMainDebug"));
        Effects[effect->name] = effect;
    }

    {
        auto effect = std::make_shared<Effect>("imgui");
        AssetPath path = "shaders/imgui.hlsl";
        effect->setPipelineType(PipelineType::Imgui);
        effect->setVertexShader(shaderManager.getVertexShader(path, "VSMain"));
        effect->setPixelShader(shaderManager.getPixelShader(path, "PSMain"));
        Effects[effect->name] = effect;
    }

    {
        auto effect = std::make_shared<Effect>("imgui_ms");
        AssetPath path = "shaders/imgui_ms.hlsl";
        effect->setPipelineType(PipelineType::Imgui);
        effect->setVertexShader(shaderManager.getVertexShader(path, "VSMain"));
        effect->setPixelShader(shaderManager.getPixelShader(path, "PSMain"));
        Effects[effect->name] = effect;
    }

    {
        auto effect = std::make_shared<Effect>("imgui_ms_depth");
        AssetPath path = "shaders/imgui_ms_depth.hlsl";
        effect->setPipelineType(PipelineType::Imgui);
        effect->setVertexShader(shaderManager.getVertexShader(path, "VSMain"));
        effect->setPixelShader(shaderManager.getPixelShader(path, "PSMain"));
        Effects[effect->name] = effect;
    }

    {
        auto effect = std::make_shared<Effect>("imgui_custom");
        effect->setPipelineType(PipelineType::Imgui);
        effect->setVertexShader(shaderManager.getVertexShader("shaders/imgui.hlsl", "VSMain"));
        effect->setPixelShader(shaderManager.getPixelShader("shaders/imgui_custom.hlsl", "PSMain"));
        Effects[effect->name] = effect;
    }
}

std::shared_ptr<Effect> EffectManager::getShadowEffect(Effect& baseEffect)
{
    auto shadowEffectName = baseEffect.name + "_shadow";
    auto* value = Effects.find_value(shadowEffectName);
    if (value)
    {
        return *value;
    }

    auto effect = std::make_shared<Effect>(shadowEffectName);
    effect->setPipelineType(PipelineType::Shadow);
    effect->setVertexShader(baseEffect.getVertexShader());
    effect->setPixelShader(baseEffect.getPixelShader()); // TODO: not always
    Effects[effect->name] = effect;

    return effect;
}
