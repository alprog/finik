export module Finik.Core.Color;

export struct Color
{
    static const Color White;
    static const Color Black;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Magenta;
    static const Color Cyan;

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

const Color Color::Black{0, 0, 0};
const Color Color::White{1, 1, 1};
const Color Color::Red{1, 0, 0};
const Color Color::Green{0, 1, 0};
const Color Color::Blue{0, 0, 1};
const Color Color::Yellow{1, 1, 0};
const Color Color::Magenta{1, 0, 1};
const Color Color::Cyan{0, 1, 1};