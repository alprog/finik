module math:Vector3;

import std;

const Vector3 Vector3::Zero{0, 0, 0};
const Vector3 Vector3::One{1, 1, 1};
const Vector3 Vector3::Forward{1, 0, 0};
const Vector3 Vector3::Backward{-1, 0, 0};
const Vector3 Vector3::Right{0, 1, 0};
const Vector3 Vector3::Left{0, -1, 0};
const Vector3 Vector3::Up{0, 0, 1};
const Vector3 Vector3::Down{0, 0, -1};

const Vector3 Vector3::East{1, 0, 0};
const Vector3 Vector3::West{-1, 0, 0};
const Vector3 Vector3::North{0, 1, 0};
const Vector3 Vector3::South{0, -1, 0};

Vector3::Vector3(float x, float y, float z)
    : x{x}
    , y{y}
    , z{z}
{
}

Vector3::Vector3(Vector2 vector, float z)
    : x{ vector.x }
    , y{ vector.y }
    , z{ z }
{
}

Vector2 Vector3::xy() const
{
    return Vector2(x, y);
}

float Vector3::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

float Vector3::squaredLength() const
{
    return x * x + y * y + z * z;
}

void Vector3::normalize()
{
    *this = getNormalized();
}

Vector3 Vector3::getNormalized()
{
    return *this / length();
}

Vector3& Vector3::operator+=(const Vector3& vector)
{
    this->x += vector.x;
    this->y += vector.y;
    this->z += vector.z;
    return *this;
}

Vector3& Vector3::operator-=(const Vector3& vector)
{
    this->x -= vector.x;
    this->y -= vector.y;
    this->z -= vector.z;
    return *this;
}

Vector3 operator-(const Vector3& vector)
{
    return {
        -vector.x,
        -vector.y,
        -vector.z};
}

Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
    return {
        lhs.x + rhs.x,
        lhs.y + rhs.y,
        lhs.z + rhs.z};
}

Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
    return {
        lhs.x - rhs.x,
        lhs.y - rhs.y,
        lhs.z - rhs.z};
}

Vector3 operator*(const Vector3& vector, const float& value)
{
    return {
        vector.x * value,
        vector.y * value,
        vector.z * value};
}

Vector3 operator/(const Vector3& vector, const float& value)
{
    return {
        vector.x / value,
        vector.y / value,
        vector.z / value};
}

Vector3 Vector3::cross(const Vector3& a, const Vector3& b)
{
    auto z = a.x * b.y - a.y * b.x; // XY
    auto x = a.y * b.z - a.z * b.y; // YZ
    auto y = a.z * b.x - a.x * b.z; // ZX
    return Vector3(x, y, z);
}

float Vector3::dot(const Vector3& lhs, const Vector3& rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}