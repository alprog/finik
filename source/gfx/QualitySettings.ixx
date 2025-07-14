export module QualitySettings;

export import Msaa;

export struct QualitySettings
{
    MSAA msaa = MSAA::Off;
    bool taa = true;

    bool operator==(const QualitySettings&) const = default;
    bool operator!=(const QualitySettings&) const = default;
};
