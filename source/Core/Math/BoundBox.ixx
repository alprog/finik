export module Finik.Core.Math:BoundBox;

import Finik.Core.Containers;
import :Vector2;
import :Vector3;
import std;

export template <typename TVector>
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

    Array<TVector> getPoints() const;
};

Array<Vector2> BoundBox<Vector2>::getPoints() const
{
    auto& _0 = min;
    auto& _1 = max;

    return {
        {_0.x, _0.y},
        {_1.x, _0.y},
        {_0.x, _1.y},
        {_1.x, _1.y},
    };
}

Array<Vector3> BoundBox<Vector3>::getPoints() const
{
    auto& _0 = min;
    auto& _1 = max;

    return {
        {_0.x, _0.y, _0.z},
        {_1.x, _0.y, _0.z},
        {_0.x, _1.y, _0.z},
        {_1.x, _1.y, _0.z},
        {_0.x, _0.y, _1.z},
        {_1.x, _0.y, _1.z},
        {_0.x, _1.y, _1.z},
        {_1.x, _1.y, _1.z}
    };
}