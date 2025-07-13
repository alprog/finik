export module Math:LineSegment;

import :Vector2;

export struct LineSegment
{
    Vector2 start;
    Vector2 end;

    float CalcSignedAreaUnderSegment() const
    {
        float averageHeight = (start.y + end.y) / 2.0f;
        float deltaX = end.x - start.x;
        return deltaX * averageHeight;
    }
};