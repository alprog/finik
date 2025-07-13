export module actor;

import Math;
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