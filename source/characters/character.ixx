module;
export module character;

import core;
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


