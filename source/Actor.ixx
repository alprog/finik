export module Actor;

import Finik.Core.Math;
import Mesh;
import Material;

export class Actor
{
public:
    Actor();

    Matrix transformMatrix;
    Matrix oldTransformMatrix;
    Mesh* mesh;
    Material* material;
};
