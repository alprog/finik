export module quality_settings;

export import msaa;

export struct QualitySettings
{
    MSAA msaa = MSAA::Off;
};

export bool operator==(const QualitySettings& lhs, const QualitySettings& rhs)
{
    return lhs.msaa == rhs.msaa;
}

export bool operator!=(const QualitySettings& lhs, const QualitySettings& rhs)
{
    return !(lhs == rhs);
}