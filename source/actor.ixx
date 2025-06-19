export module actor;

import math;
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