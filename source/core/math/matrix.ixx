export module Math:Matrix;

import :Vector4;
import :Quaternion;
import :Matrix3x3;

export struct Matrix
{
    static const Matrix Identity;

    union
    {
        struct // row-major
        {
            float m11, m12, m13, m14;
            float m21, m22, m23, m24;
            float m31, m32, m33, m34;
            float m41, m42, m43, m44;
        };

        float m[4][4]; // [row][column]
        
        Vector4 rows[4];
    };

    static Matrix Translation(Vector3 translation);
    static Matrix Rotation(Quaternion rotation);
    static Matrix RotationX(float rotation);
    static Matrix RotationY(float rotation);
    static Matrix RotationZ(float rotation);
    static Matrix Scaling(Vector3 scaling);

    static Matrix TRS(Vector3& translation, Quaternion& rotation, Vector3& scaling);

    static Matrix OrthographicLH(float width, float height, float znear, float zfar);
    static Matrix OrthographicLH(float left, float right, float bottom, float top, float znear, float zfar);

    friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
    friend Vector4 operator*(const Vector4& lhs, const Matrix& rhs);

    Vector3 MultiplyPoint(const Vector3& point);
    Vector3 MultiplyDirection(const Vector3& direction);

    Matrix& operator*=(const Matrix& rhs);
    
    void transpose();
    Matrix getTransposed();

    Matrix getInverse() const;
    Matrix getAdjugateMatrix() const;
    Matrix3x3 getMinorMatrix(int exceptRow, int exceptCol) const;
    void scaleComponents(float scalar);
};
