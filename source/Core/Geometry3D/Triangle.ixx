export module Finik.Core.Geometry3D:Triangle;

import Finik.Core.Math;

export struct Triangle
{
    Triangle(Vector3 A, Vector3 B, Vector3 C)
        : A{A}
        , B{B}
        , C{C}
    {
    }

    union
    {
        struct
        {
            Vector3 A;
            Vector3 B;
            Vector3 C;
        };
        Vector3 points[3];
    };
};