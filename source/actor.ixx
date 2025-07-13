export module actor;

import Finik.Core.Math;
import mesh;
import material;

export class Actor
{
public:
    Actor();

    void render();

    Matrix transformMatrix;
    Matrix oldTransformMatrix;
    Mesh* mesh;
    Material* material;
};