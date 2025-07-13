export module jitter;

import Finik.Core;

export Vector2 GetJitter(IntSize resolution, int32 frameIndex)
{
    static Array<Vector2> HaltonSequence = {
        {0.5000f, 0.3333f},
        {0.2500f, 0.6667f},
        {0.7500f, 0.1111f},
        {0.1250f, 0.4444f},
        {0.6250f, 0.7778f},
        {0.3750f, 0.2222f},
        {0.8750f, 0.5556f},
        {0.0625f, 0.8889f},
        {0.5625f, 0.0370f},
        {0.3125f, 0.3704f},
        {0.8125f, 0.7037f},
        {0.1875f, 0.1481f},
        {0.6875f, 0.4815f},
        {0.4375f, 0.8148f},
        {0.9375f, 0.2593f},
        {0.0313f, 0.5926f}
    };

    Vector2 value = HaltonSequence[frameIndex % HaltonSequence.count()] - Vector2(0.5f, 0.5f); // -0.5..0.5
    return Vector2{value.x / resolution.width, value.y / resolution.height} * 2.0f;
}