module Character;

import Assets;
import Texture;
import EffectManager;
import LineBuilder;
import Images;
import Finik.Core.Geometry;

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

    
    std::shared_ptr image = Images::loadPng(Path::combine("C:/finik/assets", texturePath));
    MarchingSquares squares(*image, 128);
    squares.run();

    LineBuilder builder;
    builder.setColor(Color::Red);
    builder.line({0, -0.5, 0}, {0, 0.5, 0});
    builder.line({0, -0.5, 2}, {0, 0.5, 2});
    builder.line({0, -0.5, 0}, {0, -0.5, 2});
    builder.line({0, 0.5, 0}, {0, 0.5, 2});

    auto mult = [](Vector2 p) {
        p = p / 64.0f;
        p.y = 1 - p.y;
        return Vector3(0, 0.5 - p.x, p.y * 2);
    };

    for (auto& contour : squares.getContours())
    {
        auto count = contour.points.count();
        for (int i = 0; i < count; i++)
        {
            auto a = contour.points[i];
            auto b = contour.points[(i + 1) % count];
            builder.line(mult(a), mult(b));
        }
    }

    debugLines = builder.build();
}
