export module math:BoundBox;

import :Vector2;
import :Vector3;
import std;

export template<typename TVector> 
struct BoundBox
{
    BoundBox::BoundBox(Vector2 pos)
        : min{pos}
        , max{pos}
    {
    }

    template <typename... Args>
    BoundBox::BoundBox(TVector pos, Args... args)
        : BoundBox{pos}
    {
        (growToInclude(args), ...);
    }

    TVector center() const
    {
        return min + size() / 2.0f;
    }

    TVector size() const
    {
        return max - min;
    }

    void growToInclude(TVector pos)
    {
        min = Vector2::min(min, pos);
        max = Vector2::max(max, pos);
    }

    TVector min;
    TVector max;
};