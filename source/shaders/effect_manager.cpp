module effect_manager;

import shader_manager;
import asset_path;

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
        auto effect = std::make_shared<Effect>("grid");
        AssetPath path = "shaders/grid.hlsl";
        effect->setVertexShader(shaderManager.getVertexShader(path, "VSMain"));
        effect->setPixelShader(shaderManager.getPixelShader(path, "PSMain"));
        Effects[effect->name] = effect;
    }

    {
        auto effect = std::make_shared<Effect>("directional_light");
        AssetPath path = "shaders/directional_light.hlsl";
        effect->setVertexShader(shaderManager.getVertexShader(path, "VSMain"));
        effect->setPixelShader(shaderManager.getPixelShader(path, "PSMain"));
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
    Effects[effect->name] = effect;

    return effect;
}