module Character;

import Assets;
import Texture;
import EffectManager;

Character::Character()
{
    String texturePath = "textures/body.png";
    String effectName = "standard";
    bodyMesh = createBodyMesh();
    
    if (true)
    {
        texturePath = "textures/test1.png";
        effectName = "3dsprite";
        bodyMesh = createBodyQuad();
    }
        
    transformMatrix = Matrix::Identity;

    auto texture = Assets::GetInstance().get<Texture>(texturePath.c_str());
    material = new Material("actor");
    material->Effect = EffectManager::GetInstance().get(effectName);
    material->ShadowEffect = EffectManager::GetInstance().getShadowEffect(*material->Effect);
    material->Textures.append(texture);
    material->RefreshBuffer();
}
