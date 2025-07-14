module;
export module Character;

import Finik.Core;
import Mesh;
import Material;

export class Character
{
public:
    Character();

    Matrix transformMatrix;
    Mesh* bodyMesh;
    Material* material;
};


