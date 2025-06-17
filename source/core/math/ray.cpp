module math:Ray;

import std;

Ray::Ray(Vector3 origin, Vector3 direction)
    : Origin { origin }
    , Direction { direction }
{
}

Vector3 Ray::castToGroundPlane() const
{
    if (Direction.z != 0)
    {
        auto distance = Origin.z / -Direction.z;
        return Origin + Direction * distance;
    }
    throw;
}
