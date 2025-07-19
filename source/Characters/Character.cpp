module Character;

import Assets;
import Texture;
import EffectManager;
import LineBuilder;

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

    LineBuilder builder;
    builder.setColor(Color::Red);
    builder.line({0, -0.5, 0}, {0, 0.5, 0});
    builder.line({0, -0.5, 2}, {0, 0.5, 2});
    builder.line({0, -0.5, 0}, {0, -0.5, 2});
    builder.line({0, 0.5, 0}, {0, 0.5, 2});
    debugLines = builder.build();
}
