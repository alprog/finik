export module geometry:Polygon;

import Math;
import containers;

export struct Polygon
{
    Array<Vector2> points;

    int32 count() const { return points.count(); }

    LineSegment GetSegment(int32 index) const
    {
        return {points[index], points[(index + 1) % count()]};
    }

    bool IsPositiveRotation() const
    {
        return CalcSignedArea() > 0;
    }

    float CalcSignedArea() const
    {
        float result = 0;
        for (int32 i = 0; i < count(); i++)
        {
            result += GetSegment(i).CalcSignedAreaUnderSegment();
        }
        return result;
    }
};