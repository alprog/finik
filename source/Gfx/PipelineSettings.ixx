export module PipelineSettings;

import Finik.Core;
import RenderSystemFwd;
import Shader;
import Msaa;

// for intellisense

export enum class PipelineType : char
{
    Geometry,
    Shadow,
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
