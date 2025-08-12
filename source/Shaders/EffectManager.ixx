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

    Ptr<Effect> get(String key)
    {
        if (Effects.empty())
        {
            init();
        }
        return Effects[key];
    }

    Ptr<Effect> getShadowEffect(Effect& baseEffect);

    void onShaderChanged(Ptr<Shader> shader)
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
    HashMap<String, Ptr<Effect>> Effects;
};
