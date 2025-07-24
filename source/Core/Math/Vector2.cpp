module Finik.Core.Math:Vector2;

import std;

const Vector2 Vector2::Zero {0, 0};
const Vector2 Vector2::One {1, 1};

const Vector2 Vector2::East{1, 0};
const Vector2 Vector2::West{-1, 0};
const Vector2 Vector2::North{0, -1};
const Vector2 Vector2::South{0, 1};

Vector2::Vector2()
{

}

Vector2::Vector2(float x, float y)
    : x{x}
    , y{y}
{
}

float Vector2::length()
{
    return std::sqrt(x * x + y * y);
}

float Vector2::squaredLength()
{
    return x * x + y * y;
}

Vector2 operator-(const Vector2& vector)
{
    return
    {
        -vector.x,
        -vector.y
    };
}

Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
    return
    {
        lhs.x + rhs.x,
        lhs.y + rhs.y
    };
}

Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
    return
    {
        lhs.x - rhs.x,
        lhs.y - rhs.y
    };
}

Vector2 operator*(const Vector2& vector, const float& value)
{
    return
    {
        vector.x * value,
        vector.y * value
    };
}

Vector2 operator/(const Vector2& vector, const float& value)
{
    return {
        vector.x / value,
        vector.y / value
    };
}

Vector2 Vector2::min(const Vector2& a, const Vector2& b)
{
    return {std::min(a.x, b.x), std::min(a.y, b.y)};
}

Vector2 Vector2::max(const Vector2& a, const Vector2& b)
{
    return {std::max(a.x, b.x), std::max(a.y, b.y)};
}