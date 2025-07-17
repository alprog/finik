export module Finik.Core.Math:IntSize;

import Finik.Core.PrimitiveTypes;

export struct IntSize
{
public:
    int32 width;
    int32 height;

    friend bool operator==(const IntSize& a, const IntSize& b) = default;
    friend bool operator!=(const IntSize& a, const IntSize& b) = default;
};
