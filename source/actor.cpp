module Actor;

import Mesh;
import Material;
import Texture;
import Assets;
import EffectManager;

Actor::Actor()
{
    mesh = createCubeMesh();
    transformMatrix = Matrix::Identity;
    oldTransformMatrix = transformMatrix;

    auto texture = Assets::GetInstance().get<Texture>("textures/skullbox.png");
    material = new Material("actor");
    material->Effect = EffectManager::GetInstance().get("standard");
    material->ShadowEffect = EffectManager::GetInstance().getShadowEffect(*material->Effect);
    material->Textures.append(texture);
    material->RefreshBuffer();
}
