module Character;

import Assets;
import Texture;
import EffectManager;
import LineBuilder;
import Images;
import Finik.Core.Geometry;
import SpriteManager;

Character::Character()
{
    String texturePath = "textures/body.png";
    String effectName = "standard";
    
    if (true)
    {
        texturePath = "textures/test1.png";
        effectName = "3dsprite";
    }
     
    transformMatrix = Matrix::Identity;

    auto texture = Assets::GetInstance().get<Texture>(texturePath.c_str());
    material = new Material("actor");
    material->Effect = EffectManager::GetInstance().get(effectName);
    material->ShadowEffect = EffectManager::GetInstance().getShadowEffect(*material->Effect);
    material->Textures.append(texture);
    material->RefreshBuffer();

    
    sprite = SpriteManager::GetInstance().get(texture->getPath());
    
    /*std::shared_ptr image = Images::loadPng(Path::combine("C:/finik/assets", texturePath));
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

    auto& contours = squares.getContours();

    PolygonSimplifier simplifier;
    simplifier.simplify(contours, 30);

    Triangulator triangulator;
    for (auto& simplePolygon : contours)
    {
        triangulator.addPolygon(simplePolygon);
    }
    triangulator.run();

    auto& vertices = triangulator.getVertices();
    builder.setColor(Color::Blue);
    for (auto& triangle : triangulator.getTriangles())
    {
        auto a = vertices[triangle.vertices[0]];
        auto b = vertices[triangle.vertices[1]];
        auto c = vertices[triangle.vertices[2]];
        builder.line(mult(a), mult(b));
        builder.line(mult(b), mult(c));
        builder.line(mult(c), mult(a));
        break;
    }    

    builder.setColor(Color::Red);
    for (auto& simplePolygon : contours)
    {       
        auto count = simplePolygon.points.count();
        for (int i = 0; i < count; i++)
        {
            auto a = simplePolygon.points[i];
            auto b = simplePolygon.points[(i + 1) % count];
            builder.line(mult(a), mult(b));
        }
    }

    debugLines = builder.build();*/
}
