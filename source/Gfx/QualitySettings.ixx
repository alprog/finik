export module QualitySettings;

export import Msaa;

export struct QualitySettings
{
    MSAA msaa = MSAA::Off;
    bool taa = false;
    bool shadowSnapping = true;
    int shadowMapResolution = 2048;
    int pcfSize = 3;

    bool operator==(const QualitySettings&) const = default;
    bool operator!=(const QualitySettings&) const = default;
};
