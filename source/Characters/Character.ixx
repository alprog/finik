module;
export module Character;

import Finik.Core;
import Mesh;
import Material;
import VertexBuffer;
import Sprite3D;

// for intellisense

export class Character
{
public:
    Character();

    Matrix transformMatrix;
    Material* material;

    LineVertexBuffer* debugLines;

    std::shared_ptr<Sprite3D> sprite;
};


