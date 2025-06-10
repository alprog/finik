export module surface_size;

import core;

export struct SurfaceSize
{
    IntSize resolution() const { return { width, height }; }

    int32 width;
    int32 height;
    int32 sampleCount;

    bool operator==(const SurfaceSize&) const = default;
    bool operator!=(const SurfaceSize&) const = default;
};