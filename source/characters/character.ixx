module;
export module character;

import Finik.Core;
import mesh;
import material;

export class Character
{
public:
    Character();

    Matrix transformMatrix;
    Mesh* bodyMesh;
    Material* material;
};


