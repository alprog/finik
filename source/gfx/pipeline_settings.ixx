export module pipeline_settings;

import core;
import render_system_fwd;
import shader;

export enum class PipelineType : char
{
    Geometry,
    Shadow,
    Imgui
};

export enum class CullMode : char
{
    None,
    Front,
    Back
};

export enum class MSAA : char
{
    Off = 1,
    x2 = 2,
    x4 = 4,
    x8 = 8
};

export class PipelineSettings
{
public:
    PipelineSettings() = default;
    PipelineSettings(ShaderByteCode vertexByteCode, ShaderByteCode pixelByteCode);

    PipelineType type = PipelineType::Geometry;

    ShaderByteCode vertexByteCode;
    ShaderByteCode pixelByteCode;

    CullMode cullMode;
    MSAA msaa;

    inline friend bool operator==(const PipelineSettings& lhs, const PipelineSettings& rhs) = default;
};

export template <>
struct std::hash<PipelineSettings>
{
    size_t operator()(const PipelineSettings& settings) const
    {
        return Hash::getRawMemoryHash(this);
    }
};
