export module Finik.Core.Geometry:Polygon;

import Finik.Core.Math;
import Finik.Core.Containers;

export struct Polygon
{
    Array<Vector2> points;

    int32 count() const { return points.count(); }

    LineSegment getSegment(int32 index) const
    {
        return {points[index], points[(index + 1) % count()]};
    }

    bool isPositiveRotation() const
    {
        return calcSignedArea() > 0;
    }

    float calcSignedArea() const
    {
        float result = 0;
        for (int32 i = 0; i < count(); i++)
        {
            result += getSegment(i).calcSignedAreaUnderSegment();
        }
        return result;
    }
};
