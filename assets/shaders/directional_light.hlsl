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

float4 restoreWorldPosition(float2 uv)
{
	float x = uv.x * 2 - 1;
    float y = 1 - uv.y * 2;
    float z = sampleTex(DSId, uv).r;
    float4 ndcPos = float4(x, y, z, 1);
    float4 worldPos = mul(ndcPos, InverseViewProjection);
    return worldPos / worldPos.w;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 albedo = sampleTex(RT0Id, input.uv);
	float3 normal = sampleTex(RT1Id, input.uv).rgb;

	normal = normalize(normal * 2 - 1);

	float4 ambient = float4(0.2, 0.2, 0.2, 1);

	float diffuse = dot(normal, -LightDirection.xyz);	

	float4 worldPos = restoreWorldPosition(input.uv);
	worldPos = worldPos / worldPos.w;
		
	float4 shadowPos = mul(worldPos, ShadowViewProjection);
	shadowPos = shadowPos / shadowPos.w;
	float2 shadowUV = float2(shadowPos.x / 2 + 0.5, 0.5 - shadowPos.y / 2);
	float shadowValue = sampleTex(ShadowTextureId, shadowUV).r;
	
	float linearShadowValue = LinearizeDepth(shadowValue, 0.1, 400);
	
	if (shadowPos.z > shadowValue + 0.00016)
	{
		diffuse = 0.1;
	}
	
	return ambient + diffuse * albedo;
}