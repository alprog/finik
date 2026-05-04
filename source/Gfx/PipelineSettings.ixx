export module PipelineSettings;

import Finik.Core;
import Shader;
import GfxEnums;

export enum class PipelineType : char
{
    Geometry,
    Shadow,
    ShadowPost,
    Lighting,
    DebugLines,
    ScreenSpace,
    Imgui
};

export enum class CullMode : char
{
    None,
    Front,
    Back
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
    TextureFormat shadowFormat;

    bool operator==(const PipelineSettings& lhs) const = default;
    bool operator!=(const PipelineSettings& lhs) const = default;
};

export template <>
struct std::hash<PipelineSettings>
{
    size_t operator()(const PipelineSettings& settings) const
    {
        return Hash::getRawMemoryHash(this);
    }
};
