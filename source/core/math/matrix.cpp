module Math:Matrix;

import std;

const Matrix Matrix::Identity =
{
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
};

Matrix operator*(const Matrix& lhs, const Matrix& rhs)
{
    return // mij = mi1*m1j + mi2*m2j + mi3*m3j + mi4*m4j
    {
        lhs.m11 * rhs.m11 + lhs.m12 * rhs.m21 + lhs.m13 * rhs.m31 + lhs.m14 * rhs.m41,
        lhs.m11 * rhs.m12 + lhs.m12 * rhs.m22 + lhs.m13 * rhs.m32 + lhs.m14 * rhs.m42,
        lhs.m11 * rhs.m13 + lhs.m12 * rhs.m23 + lhs.m13 * rhs.m33 + lhs.m14 * rhs.m43,
        lhs.m11 * rhs.m14 + lhs.m12 * rhs.m24 + lhs.m13 * rhs.m34 + lhs.m14 * rhs.m44,

        lhs.m21 * rhs.m11 + lhs.m22 * rhs.m21 + lhs.m23 * rhs.m31 + lhs.m24 * rhs.m41,
        lhs.m21 * rhs.m12 + lhs.m22 * rhs.m22 + lhs.m23 * rhs.m32 + lhs.m24 * rhs.m42,
        lhs.m21 * rhs.m13 + lhs.m22 * rhs.m23 + lhs.m23 * rhs.m33 + lhs.m24 * rhs.m43,
        lhs.m21 * rhs.m14 + lhs.m22 * rhs.m24 + lhs.m23 * rhs.m34 + lhs.m24 * rhs.m44,

        lhs.m31 * rhs.m11 + lhs.m32 * rhs.m21 + lhs.m33 * rhs.m31 + lhs.m34 * rhs.m41,
        lhs.m31 * rhs.m12 + lhs.m32 * rhs.m22 + lhs.m33 * rhs.m32 + lhs.m34 * rhs.m42,
        lhs.m31 * rhs.m13 + lhs.m32 * rhs.m23 + lhs.m33 * rhs.m33 + lhs.m34 * rhs.m43,
        lhs.m31 * rhs.m14 + lhs.m32 * rhs.m24 + lhs.m33 * rhs.m34 + lhs.m34 * rhs.m44,

        lhs.m41 * rhs.m11 + lhs.m42 * rhs.m21 + lhs.m43 * rhs.m31 + lhs.m44 * rhs.m41,
        lhs.m41 * rhs.m12 + lhs.m42 * rhs.m22 + lhs.m43 * rhs.m32 + lhs.m44 * rhs.m42,
        lhs.m41 * rhs.m13 + lhs.m42 * rhs.m23 + lhs.m43 * rhs.m33 + lhs.m44 * rhs.m43,
        lhs.m41 * rhs.m14 + lhs.m42 * rhs.m24 + lhs.m43 * rhs.m34 + lhs.m44 * rhs.m44
    };
}

Vector4 operator*(const Vector4& v, const Matrix& m)
{
    return // mij = mi1*m1j + mi2*m2j + mi3*m3j + mi4*m4j
    {
        v.x * m.m11 + v.y * m.m21 + v.z * m.m31 + v.w * m.m41,
        v.x * m.m12 + v.y * m.m22 + v.z * m.m32 + v.w * m.m42,
        v.x * m.m13 + v.y * m.m23 + v.z * m.m33 + v.w * m.m43,
        v.x * m.m14 + v.y * m.m24 + v.z * m.m34 + v.w * m.m44,
    };
}

Vector3 Matrix::MultiplyPoint(const Vector3& point)
{
    return
    {
        point.x * m11 + point.y * m21 + point.z * m31 + 1 * m41,
        point.x * m12 + point.y * m22 + point.z * m32 + 1 * m42,
        point.x * m13 + point.y * m23 + point.z * m33 + 1 * m43
    };
}

Vector3 Matrix::MultiplyDirection(const Vector3& direction)
{
    return
    {
        direction.x * m11 + direction.y * m21 + direction.z * m31 + 0 * m41,
        direction.x * m12 + direction.y * m22 + direction.z * m32 + 0 * m42,
        direction.x * m13 + direction.y * m23 + direction.z * m33 + 0 * m43
    };
}

Matrix& Matrix::operator*=(const Matrix& rhs)
{
    (*this) = (*this) * rhs;
    return *this;
}

Matrix Matrix::Translation(Vector3 t)
{
    return
    {
        1,   0,   0,   0,
        0,   1,   0,   0,
        0,   0,   1,   0,
       t.x, t.y, t.z,  1
    };
}

Matrix Matrix::Rotation(Quaternion rotation)
{
    auto xx2 = rotation.x * rotation.x * 2;
    auto xy2 = rotation.x * rotation.y * 2;
    auto xz2 = rotation.x * rotation.z * 2;
    auto xw2 = rotation.x * rotation.w * 2;

    auto yy2 = rotation.y * rotation.y * 2;
    auto yz2 = rotation.y * rotation.z * 2;
    auto yw2 = rotation.y * rotation.w * 2;

    auto zz2 = rotation.z * rotation.z * 2;
    auto zw2 = rotation.z * rotation.w * 2;

    return
    {
      1 - (yy2 + zz2), xy2 + zw2, xz2 - yw2, 0,
      xy2 - zw2, 1 - (xx2 + zz2), yz2 + xw2, 0,
      xz2 + yw2, yz2 - xw2, 1 - (xx2 + yy2), 0,
      0, 0, 0, 1
    };
}

Matrix Matrix::RotationX(float r)
{
    float c = std::cos(r);
    float s = std::sin(r);

    return
    {
        1,  0,  0,  0,
        0, +c, +s,  0,
        0, -s, +c,  0,
        0,  0,  0,  1
    };
}

Matrix Matrix::RotationY(float r)
{
    float c = std::cos(r);
    float s = std::sin(r);

    return
    {
       +c,  0, -s,  0,
        0,  1,  0,  0,
       +s,  0, +c,  0,
        0,  0,  0,  1
    };
}

Matrix Matrix::RotationZ(float r)
{
    float c = std::cos(r);
    float s = std::sin(r);

    return
    {
       +c, +s,  0,  0,
       -s, +c,  0,  0,
        0,  0,  1,  0,
        0,  0,  0,  1
    };
}

Matrix Matrix::Scaling(Vector3 s)
{
    return
    {
       s.x,  0,   0,   0,
        0,  s.y,  0,   0,
        0,   0,  s.z,  0,
        0,   0,   0,   1
    };
}

Matrix Matrix::TRS(Vector3& translation, Quaternion& rotation, Vector3& scaling)
{
    return Scaling(scaling) * Rotation(rotation) * Translation(translation);
}

Matrix Matrix::OrthographicLH(float width, float height, float znear, float zfar)
{
    auto d = zfar - znear;

    return
    {
       2/width,    0,       0,       0,
          0,    2/height,   0,       0,
          0,       0,      1/d,      0,
          0,       0,    -znear/d,   1
    };
}

Matrix Matrix::OrthographicLH(float left, float right, float bottom, float top, float znear, float zfar)
{
    auto w = right - left;
    auto h = top - bottom;
    auto d = zfar - znear;

    auto a = -(left+right)/w;
    auto b = -(top+bottom)/h;

    return
    {
       2/w,   0,      0,      0,
        0,   2/h,     0,      0,
        0,    0,     1/d,     0,
        a,    b,   -znear/d,  1,
    };
}

void Matrix::transpose()
{
    *this = getTransposed();
}

Matrix Matrix::getTransposed()
{
    return {
        m[0][0], m[1][0], m[2][0], m[3][0],
        m[0][1], m[1][1], m[2][1], m[3][1],
        m[0][2], m[1][2], m[2][2], m[3][2],
        m[0][3], m[1][3], m[2][3], m[3][3]
    };
}

Matrix Matrix::getInverse() const
{
    auto matrix = getAdjugateMatrix();
    float det = 0;
    for (auto i = 0; i < 4; i++)
    {
        det += m[0][i] * matrix.m[i][0];
    }
    matrix.scaleComponents(1.0f / det);
    return matrix;
}

Matrix Matrix::getAdjugateMatrix() const
{
    Matrix adjMatrix;

    for (auto row = 0; row < 4; row++)
    {
        for (auto col = 0; col < 4; col++)
        {
            auto determinant = getMinorMatrix(row, col).determinant();
            auto sign = (row + col) % 2 ? -1 : 1;
            adjMatrix.m[col][row] = determinant * sign;
        }
    }

    return adjMatrix;
}

Matrix3x3 Matrix::getMinorMatrix(int exceptRow, int exceptCol) const
{
    Matrix3x3 minorMtrx;
 
    const float* src = &m[0][0];
    float* dst = &minorMtrx.m[0][0];

    for (int i = 0; i < 16; i++)
    {
        if (i / 4 != exceptRow && i % 4 != exceptCol)
        {
            *dst++ = src[i];
        }
    }
    
    return minorMtrx;
}

void Matrix::scaleComponents(float scalar)
{
    for (Vector4& row : rows)
    {
        row = row * scalar;
    }
}