module;
export module Character;

import Finik.Core;
import Mesh;
import Material;
import VertexBuffer;

// for intellisense

export class Character
{
public:
    Character();

    Matrix transformMatrix;
    Mesh* bodyMesh;
    Material* material;

    LineVertexBuffer* debugLines;
};


