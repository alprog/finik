export module Finik.Core.Math:CoordinateSystem;

import :Vector3;
import :Matrix3x3;
import std;

enum SemanticDirection : char
{
    Right = 0,
    Left = 1,
    Up = 2,
    Down = 3,
    Forward = 4,
    Backward = 5,

    Count
};

SemanticDirection getSemanticDirection(char c)
{
    switch (std::toupper(c))
    {
        case 'R': return SemanticDirection::Right;
        case 'L': return SemanticDirection::Left;
        case 'F': return SemanticDirection::Forward;
        case 'B': return SemanticDirection::Backward;
        case 'U': return SemanticDirection::Up;
        case 'D': return SemanticDirection::Down;

        default:
            throw "unknown semantic direction";
    }
}

export class CoordinateSystem
{
public:
    static const CoordinateSystem World;

    constexpr CoordinateSystem(char axes[3])
    {
        for (int i = 0; i < 3; i++)
        {
            Vector3 baseVector = Vector3::Zero;
            baseVector.components[i] = 1.0f;

            auto dir = getSemanticDirection(axes[i]);
            semanticDirections[dir] = baseVector;
            semanticDirections[dir ^ 1] = -baseVector;
        }
    }

    Matrix3x3 getConversionMatrixFrom(char fromAxes[3]) const
    {
        Matrix3x3 matrix;
        for (int i = 0; i < 3; i++)
        {
            auto dir = getSemanticDirection(fromAxes[i]);
            matrix.rows[i] = semanticDirections[dir];
        }
        return matrix;
    }

    Matrix3x3 getConversionMatrixTo(char toAxes[3]) const
    {
        return getConversionMatrixFrom(toAxes).getTransposed();
    }

    static Matrix3x3 getConversionMatrix(char fromAxes[3], char toAxes[3])
    {
        return CoordinateSystem(fromAxes).getConversionMatrixTo(toAxes);
    }

private:
    Vector3 semanticDirections[SemanticDirection::Count];
};

const CoordinateSystem CoordinateSystem::World{"RFU"};