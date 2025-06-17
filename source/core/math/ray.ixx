export module math:Ray;

import :Vector3;

export struct Ray
{
    Ray() = default;
    Ray(Vector3 origin, Vector3 direction);

    Vector3 castToGroundPlane() const;

    Vector3 Origin;
    Vector3 Direction;
};