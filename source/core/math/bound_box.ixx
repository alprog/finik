export module math:BoundBox;

import :Vector2;
import std;

export struct BoundBox
{
    BoundBox::BoundBox(Vector2 pos)
        : min{pos}
        , max{pos}
    {
    }

    template <typename... Args>
    BoundBox::BoundBox(Vector2 pos, Args... args)
        : BoundBox{pos}
    {
        (growToInclude(args), ...);
    }

    Vector2 center() const
    {
        return min + size() / 2.0f;
    }

    Vector2 size() const
    {
        return max - min;
    }

    void growToInclude(Vector2 pos)
    {
        min = Vector2::min(min, pos);
        max = Vector2::max(max, pos);
    }

    Vector2 min;
    Vector2 max;
};