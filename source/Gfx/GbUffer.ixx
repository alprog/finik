export module GBuffer;

import Finik.Core;
import DX;
import FrameBuffer;

// for intellisense

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
            TextureFormat::DXGI_FORMAT_R16G16_SNORM,
            TextureFormat::DXGI_FORMAT_R8G8B8A8_UNORM
        };
        return formats;
    }
};
