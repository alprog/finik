module character;

import assets;
import texture;
import effect_manager;

Character::Character()
{
    bodyMesh = createBodyMesh();
    transformMatrix = Matrix::Identity;

    auto texture = Assets::GetInstance().get<Texture>("textures/body.png");
    material = new Material("actor");
    material->Effect = EffectManager::GetInstance().get("standard");
    material->ShadowEffect = EffectManager::GetInstance().getShadowEffect(*material->Effect);
    material->Textures.append(texture);
    material->RefreshBuffer();
}