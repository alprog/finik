export module Finik.Core.Math:BoundBox;

import :Vector2;
import :Vector3;
import std;

export template<typename TVector> 
struct BoundBox
{
    TVector min;
    TVector max;

    BoundBox::BoundBox(Vector2 pos)
        : min{pos}
        , max{pos}
    {
    }

    template <typename... Args>
    BoundBox::BoundBox(TVector pos, Args... args)
        : min{pos}
        , max{pos}
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
        min = TVector::min(min, pos);
        max = TVector::max(max, pos);
    }
};