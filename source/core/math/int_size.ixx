export module Math:IntSize;

import primitive_types;

export struct IntSize
{
public:
    int32 width;
    int32 height;

    friend bool operator==(const IntSize& a, const IntSize& b) = default;
    friend bool operator!=(const IntSize& a, const IntSize& b) = default;
};