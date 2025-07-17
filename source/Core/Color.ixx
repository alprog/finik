export module Finik.Core.Color;

export struct Color
{
    Color() = default;

    Color(float r, float g, float b, float a = 1)
        : R { r }
        , G { g }
        , B { b }
        , A { a }
    {
    }

    float R, G, B, A;
};

static_assert(sizeof(Color) == 16);
