export module quality_settings;

export import msaa;

export struct QualitySettings
{
    MSAA msaa = MSAA::Off;

    bool operator==(const QualitySettings&) const = default;
    bool operator!=(const QualitySettings&) const = default;
};
