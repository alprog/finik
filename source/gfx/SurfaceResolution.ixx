export module SurfaceResolution;

import Finik.Core;

// Resolution + MSAA sample count
export struct SurfaceResolution
{
    IntSize resolution() const { return { width, height }; }

    int32 width;
    int32 height;
    int32 sampleCount;

    bool operator==(const SurfaceResolution&) const = default;
    bool operator!=(const SurfaceResolution&) const = default;
};

static_assert(sizeof(SurfaceResolution) == 12);
