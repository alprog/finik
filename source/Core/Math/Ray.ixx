export module Finik.Core.Math:Ray;

import :Vector3;

export struct Ray
{
    Vector3 Origin;
    Vector3 Direction;

    Ray() = default;
    Ray(Vector3 origin, Vector3 direction);

    Vector3 castToHorizontalPlane(float planeAltitude) const;
    Vector3 castToGroundPlane() const;
};