export module QualitySettings;

export import GfxEnums;

export struct QualitySettings
{
    MSAA msaa = MSAA::Off;
    bool taa = false;
    bool shadowSnapping = true;
    int shadowMapResolution = 2048;
    ShadowMapPrecision shadowMapPrecision = ShadowMapPrecision::Bit16;   
    int pcfSize = 3;

    bool operator==(const QualitySettings&) const = default;
    bool operator!=(const QualitySettings&) const = default;

    auto getShadowMapFormat() const { return ::getShadowMapFormat(shadowMapPrecision); }
};
