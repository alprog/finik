module shader_manager;

const char* ShaderManager::getFallbackVertexShaderText()
{
    return "                                                    \
        #pragma pack_matrix( row_major )\n                      \
                                                                \
        cbuffer FrameConstantBuffer : register(b1)              \
        {                                                       \
            float4x4 ViewProject;                               \
        };                                                      \
                                                                \
        cbuffer MeshConstantBuffer : register(b2)               \
        {                                                       \
            float4x4 Model;                                     \
        };                                                      \
                                                                \
        float4 VSMain(float3 position : POSITION) : SV_POSITION \
        {                                                       \
            float4x4 MVP = mul(Model, ViewProject);             \
            return mul(float4(position, 1), MVP);               \
        }                                                       \
    ";
}

const char* ShaderManager::getFallbackPixelShaderText()
{
    return "float4 PSMain() : SV_TARGET { return float4(1, 0, 1, 1); }";
}

const char* ShaderManager::getFallbackComputeShaderText()
{
    return "[numthreads(1, 1, 1)] void CSMain(uint3 coord : SV_DispatchThreadID) {}";
}