export module g_buffer;

import core;
import dx;
import frame_buffer;

export class GBuffer : public FrameBuffer
{
public:
    GBuffer(SurfaceResolution resolution)
        : FrameBuffer{resolution, GetRTFormats(), true}
    {
    }

    static const Array<TextureFormat>& GetRTFormats()
    {
        static Array<TextureFormat> formats =
        {
            TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM,
            TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM,
            TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM,
            TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM
        };
        return formats;
    }
};