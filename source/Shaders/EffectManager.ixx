export module EffectManager;

import Finik.Core;
import Finik.Core.Singleton;
import Effect;
import Shader;

export class EffectManager : public Singleton<EffectManager>
{
    friend class QualityManager;

public:
    void init();

    std::shared_ptr<Effect> get(String key)
    {
        if (Effects.empty())
        {
            init();
        }
        return Effects[key];
    }

    std::shared_ptr<Effect> getShadowEffect(Effect& baseEffect);

    void onShaderChanged(std::shared_ptr<Shader> shader)
    {
        for (auto [_, effect] : Effects)
        {
            if (effect->getVertexShader() == shader || effect->getPixelShader() == shader)
            {
                effect->resetPso();
            }
        }
    };

private:
    HashMap<String, std::shared_ptr<Effect>> Effects;
};
