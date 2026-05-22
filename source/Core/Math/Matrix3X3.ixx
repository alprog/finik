export module Finik.Core.Math:Matrix3x3;

import :Vector3;

export struct Matrix3x3
{
    static const Matrix3x3 Identity;

    union
    {
        struct // row-major
        {
            float m11, m12, m13;
            float m21, m22, m23;
            float m31, m32, m33;
        };

        struct // row-major
        {
            float a, b, c;
            float d, e, f;
            float g, h, i;
        };

        float m[3][3]; // [row][column]

        Vector3 rows[3];
    };

    void transpose()
    {
        *this = getTransposed();
    }

    Matrix3x3 getTransposed()
    {
        return {
            m[0][0], m[1][0], m[2][0],
            m[0][1], m[1][1], m[2][1],
            m[0][2], m[1][2], m[2][2]
        };
    }

    float determinant() const
    {
        return a * e * i + b * f * g + c * d * h - c * e * g - b * d * i - a * f * h;
    }

    friend Vector3 operator*(const Vector3& v, const Matrix3x3& m)
    {
        return // mij = mi1*m1j + mi2*m2j + mi3*m3j + mi4*m4j
        {
            v.x * m.m11 + v.y * m.m21 + v.z * m.m31,
            v.x * m.m12 + v.y * m.m22 + v.z * m.m32,
            v.x * m.m13 + v.y * m.m23 + v.z * m.m33,
        };
    }
};

const Matrix3x3 Matrix3x3::Identity = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1
};