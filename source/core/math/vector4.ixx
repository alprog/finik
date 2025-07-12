export module math:Vector4;

import :Vector2;
import :Vector3;

export struct Vector4
{
    float x, y, z, w;

    static const Vector4 Zero;
    static const Vector4 One;

    Vector4() = default;
    Vector4(Vector2 vector, float z, float w);
    Vector4(Vector3 vector, float w);
    Vector4(float x, float y, float z, float w);

    Vector3 xyz();

    float length();
    float squaredLength();
    Vector4 getNormalized();

    void homoNormalize();

    friend Vector4 operator-(const Vector4& vector);
    friend Vector4 operator+(const Vector4& lhs, const Vector4& rhs);
    friend Vector4 operator-(const Vector4& lhs, const Vector4& rhs);
    friend Vector4 operator*(const Vector4& vector, float scalar);
    friend Vector4 operator/(const Vector4& vector, float scalar);

    Vector4& operator*=(float scalar);
    Vector4& operator/=(float scalar);

    static Vector4 cross(const Vector4& a, const Vector4& b);
    static float dot(const Vector4& a, const Vector4& b);
};
