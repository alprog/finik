module;
#include <asserts.h>
module Finik.Core.Math:Ray;

import std;

Ray::Ray(Vector3 origin, Vector3 direction)
    : Origin { origin }
    , Direction { direction }
{
}

Vector3 Ray::castToHorizontalPlane(float planeAltitude) const
{
    ASSERT(Direction.z != 0);

    auto travelDistance = (planeAltitude - Origin.z) / Direction.z;
    return Origin + Direction * travelDistance;
}

Vector3 Ray::castToGroundPlane() const
{
    return castToHorizontalPlane(0);
}
