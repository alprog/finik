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

float4 sampleGTex(uint textureId, float2 uv, uint sampleIndex)
{
	Texture2DMS<float4> texture = textures[textureId];
	float2 pos = uv * Resolution;
	return texture.Load(pos, sampleIndex);
}

float4 restoreWorldPosition(float2 uv, uint sampleIndex)
{
	float x = uv.x * 2 - 1;
    float y = 1 - uv.y * 2;
    float z = sampleGTex(DSId, uv, sampleIndex).r;
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

	float bias = 0.01;
	Texture2D texture = textures[ShadowTextureId];
	
	float result = 0;
	for (int x = -2; x <= 2; x++)
	{
		for (int y = -2; y <= 2; y++)
		{
			float shadowValue = texture.Sample(PointSampler, shadowUV + float2(x, y) / 2048).r;
			result += refDepth > shadowValue + bias ? 1 : 0;
		}
	}

	return result / 25;
}

float4 calcLighting(float2 uv, uint sampleIndex)
{
	float4 albedo = sampleGTex(RT0Id, uv, sampleIndex);
	float3 normal = sampleGTex(RT1Id, uv, sampleIndex).rgb;

	normal = normalize(normal * 2 - 1);

	float4 ambient = float4(0.2, 0.2, 0.2, 1);

	float diffuse = saturate(dot(normal, -LightDirection.xyz));	

	float4 worldPos = restoreWorldPosition(uv, sampleIndex);
	worldPos = worldPos / worldPos.w;
		
	float4 shadowPos = mul(worldPos, ShadowViewProjection);
	shadowPos = shadowPos / shadowPos.w;
	float2 shadowUV = float2(shadowPos.x / 2 + 0.5, 0.5 - shadowPos.y / 2);
	
	float shadow = getShadow(shadowUV, shadowPos.z);
	
	diffuse = lerp(diffuse, 0, shadow);
	
	return (ambient + diffuse) * albedo;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 color = 0;
	for (int i = 0; i < SampleCount; i++)
	{
		color += calcLighting(input.uv, i);
	}	
	color /= SampleCount;
	return color;
}