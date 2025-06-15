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

float4 sampleTex(uint textureId, float2 uv)
{
	Texture2D texture = textures[textureId];
	return texture.Sample(PointSampler, uv);
}

float4 sampleGTex(uint textureId, float2 uv)
{
	Texture2DMS<float4> texture = textures[textureId];
	uint2 resolution;
	uint sampleCount;
	texture.GetDimensions(resolution.x, resolution.y, sampleCount);

	float2 pos = uv * resolution;
	return texture.Load(pos, 0);
}

float4 restoreWorldPosition(float2 uv)
{
	float x = uv.x * 2 - 1;
    float y = 1 - uv.y * 2;
    float z = sampleGTex(DSId, uv).r;
    float4 ndcPos = float4(x, y, z, 1);
    float4 worldPos = mul(ndcPos, InverseViewProjection);
    return worldPos / worldPos.w;
}

float getShadow(float2 shadowUV, float refDepth)
{
	if (shadowUV.x < 0 || shadowUV.x > 1 || shadowUV.y < 0 || shadowUV.y > 1)
	{
		return 0;
	}

	float shadowValue = sampleTex(ShadowTextureId, shadowUV).r;
	float bias = 0.001;
	return refDepth > shadowValue + bias ? 1 : 0;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 albedo = sampleGTex(RT0Id, input.uv);
	float3 normal = sampleGTex(RT1Id, input.uv).rgb;

	normal = normalize(normal * 2 - 1);

	float4 ambient = float4(0.2, 0.2, 0.2, 1);

	float diffuse = dot(normal, -LightDirection.xyz);	

	float4 worldPos = restoreWorldPosition(input.uv);
	worldPos = worldPos / worldPos.w;
		
	float4 shadowPos = mul(worldPos, ShadowViewProjection);
	shadowPos = shadowPos / shadowPos.w;
	float2 shadowUV = float2(shadowPos.x / 2 + 0.5, 0.5 - shadowPos.y / 2);
	
	float shadow = getShadow(shadowUV, shadowPos.z);
	
	if (shadow > 0)
	{
		diffuse = 0.1;
	}
	
	return ambient + diffuse * albedo;
}