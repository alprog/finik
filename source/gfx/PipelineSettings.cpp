module PipelineSettings;

PipelineSettings::PipelineSettings(ShaderByteCode vertexByteCode, ShaderByteCode pixelByteCode)
    : type{PipelineType::Geometry}
    , vertexByteCode{vertexByteCode}
    , pixelByteCode{pixelByteCode}
    , cullMode{CullMode::Back}
    , msaa{MSAA::Off}
{
}
