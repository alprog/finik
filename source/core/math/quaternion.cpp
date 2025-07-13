module Math:Quaternion;

import std;

const Quaternion Quaternion::Identity {0, 0, 0, 1};

Quaternion::Quaternion(float x, float y, float z, float w)
    : x{x}
    , y{y}
    , z{z}
    , w{w}
{
}

Quaternion Quaternion::yawPitchRoll(float yaw, float pitch, float roll) // Euler sequence 2; -1; -3
{
    auto c1 = std::cosf(yaw / 2);
    auto s1 = std::sinf(yaw / 2);
    auto c2 = std::cosf(-pitch / 2);
    auto s2 = std::sinf(-pitch / 2);
    auto c3 = std::cosf(-roll / 2);
    auto s3 = std::sinf(-roll / 2);

    return
    {
        s1 * c2 * s3 + c1 * s2 * c3,
        s1 * c2 * c3 - c1 * s2 * s3,
       -s1 * s2 * c3 + c1 * c2 * s3,
        s1 * s2 * s3 + c1 * c2 * c3,
    };
}

Quaternion Quaternion::FromAxis(Vector3 axis, float angle)
{
    auto s = std::sin(angle / 2);
    return Quaternion(
        axis.x * s,
        axis.y * s,
        axis.z * s, 
        std::cos(angle / 2)
    );
}