export module Finik.Core.Math:Quaternion;

import :Vector3;

export class Quaternion
{
public:
    static const Quaternion Identity;

    Quaternion(float x = 0, float y = 0, float z = 0, float w = 1);

    static Quaternion yawPitchRoll(float yaw, float pitch, float roll);

    static Quaternion FromAxis(Vector3 axis, float angle);

    float x, y, z, w;
};
