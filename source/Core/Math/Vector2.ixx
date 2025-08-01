export module Finik.Core.Math:Vector2;

export struct Vector2
{
    float x, y;

    static const Vector2 Zero;
    static const Vector2 One;

    static const Vector2 East;
    static const Vector2 West;
    static const Vector2 North;
    static const Vector2 South;

    Vector2();
    Vector2(float x, float y);

    float length();
    float squaredLength();

    friend Vector2 operator-(const Vector2& vector);
    friend Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
    friend Vector2 operator-(const Vector2& lhs, const Vector2& rhs);

    friend Vector2 operator*(const Vector2& vector, const float& value);
    friend Vector2 operator/(const Vector2& vector, const float& value);

    static Vector2 min(const Vector2& a, const Vector2& b);
    static Vector2 max(const Vector2& a, const Vector2& b);
};