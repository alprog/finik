#include "shaders/common.inc"

// per draw
cbuffer LightConstantBuffer : register(b3)
{
	float4 LightDirection;
	float4x4 ShadowViewProjection;
	uint ShadowTextureId;
};

struct VSInput
{
	float3 position : POSITION;
	float4 uv : TEXCOORD0;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD1;
};

float4 screenToNDC(float3 pos)
{
	return float4(pos.x * 2 - 1, 1 - pos.y * 2, pos.z, 1);
}

float LinearizeDepth(float depth, float nearPlane, float farPlane)
{
    return (2.0 * nearPlane) / (farPlane + nearPlane - depth * (farPlane - nearPlane));
}

PSInput VSMain(VSInput input)
{
	PSInput result;
	result.position = screenToNDC(input.position);
	result.uv = input.uv;
	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	Texture2D albedoTex = textures[RT0Id];
	Texture2D normalTex = textures[RT1Id];
	
	float4 albedo = albedoTex.Sample(PointSampler, input.uv);
	float3 normal = normalTex.Sample(PointSampler, input.uv).rgb;

	normal = normal * 2 - 1;

	float3 lightDirection = float3(-1, -1, -1);
	float diffuse = dot(normal, -lightDirection);

	return diffuse * albedo;
}