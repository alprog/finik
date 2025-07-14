module Finik.Core.Math:Matrix3X3;

float Matrix3x3::determinant() const
{
    return a*e*i + b*f*g + c*d*h - c*e*g - b*d*i - a*f*h;
}
