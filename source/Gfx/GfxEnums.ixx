export module GfxEnums;

export import TextureFormat;

export enum class MSAA : int
{
    Off = 0,
    x2 = 1,
    x4 = 2,
    x8 = 3
};

export int getSampleCount(MSAA msaa)
{
    switch (msaa)
    {
    case MSAA::x2:
        return 2;
    case MSAA::x4:
        return 4;
    case MSAA::x8:
        return 8;

    case MSAA::Off:
    default:
        return 1;
    }
}

export enum class ShadowMapPrecision : int
{
    Bit8 = 0,
    Bit16 = 1,
    Bit32 = 2
};

export TextureFormat getShadowMapFormat(ShadowMapPrecision precision)
{
    switch (precision)
    {
    case ShadowMapPrecision::Bit8:
        return TextureFormat::DXGI_FORMAT_R8G8_UNORM;
    case ShadowMapPrecision::Bit16:
        return TextureFormat::DXGI_FORMAT_R16G16_UNORM;
    case ShadowMapPrecision::Bit32:
        return TextureFormat::DXGI_FORMAT_R32G32_FLOAT;
    }
    throw;
}