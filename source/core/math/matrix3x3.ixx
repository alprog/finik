export module Math:Matrix3x3;

import :Vector3;

export struct Matrix3x3
{
    float determinant() const;

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
};