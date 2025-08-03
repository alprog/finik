export module Finik.Core.Geometry:Polygon;

import Finik.Core.Math;
import Finik.Core.Containers;

export struct Polygon
{
    Array<Vector2> points;

    int32 pointCount() const { return points.count(); }
    int32 edgeCount() const {return points.count(); }

    LineSegment getSegment(int32 index) const
    {
        return {points[index], points[(index + 1) % pointCount()]};
    }

    bool isPositiveRotation() const
    {
        return calcSignedArea() > 0;
    }

    float calcSignedArea() const
    {
        float result = 0;
        for (int32 i = 0; i < pointCount(); i++)
        {
            result += getSegment(i).calcSignedAreaUnderSegment();
        }
        return result;
    }
};
