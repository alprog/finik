export module g_buffer;

import core;
import frame_buffer;

export class GBuffer : public FrameBuffer
{
public:
    GBuffer(SurfaceResolution resolution)
        : FrameBuffer{resolution, 4, true}
    {
    }
};