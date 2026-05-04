#include "shaders/common.inc"

// per draw
cbuffer LightConstantBuffer : register(b3)
{
	float4 LightDirection;
	float4x4 ShadowViewProjection;
	uint ShadowTextureId;
	uint PCFSize;
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
	return float4(pos.x * 2 - 1, pos.y * 2 - 1, pos.z, 1);
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

float4 restoreWorldPosition(float2 uv, uint sampleIndex)
{
	float x = uv.x * 2 - 1;
    float y = uv.y * 2 - 1;
    float z = sampleGTex(DSId, uv, sampleIndex).r;
    float4 ndcPos = float4(x, y, z, 1);
    float4 worldPos = mul(ndcPos, InverseViewProjection);
    return worldPos / worldPos.w;
}

float getShadow2(float2 shadowUV, float refDepth, float bias)
{
	if (shadowUV.x < 0 || shadowUV.x > 1 || shadowUV.y < 0 || shadowUV.y > 1)
	{
		return 0;
	}
	
	int s = PCFSize;
	
	float result = 0;
	int count = 0;
	for (int x = -s; x <= s; x++)
	{
		for (int y = -s; y <= s; y++)
		{
            float shadowValue = sampleTex(ShadowTextureId, shadowUV + float2(x, y) / 2048, PointSampler).r;
			result += refDepth > shadowValue + bias ? 1 : 0;
			count += 1;
		}
	}

	return result / count;
}

float reduceLightBleeding(float value, float min)  
{  
    return clamp((value - min) / (1 - min), 0, 1);  
}  

float getShadow(float2 uv, float refDepth, float bias)  
{  
    if (uv.x < 0 || uv.x > 1 || uv.y < 0 || uv.y > 1)
    {
        return 1;
    }

    float2 M = sampleTex(ShadowTextureId, uv, LinearSampler).rg;

    float p = refDepth <= M[0];
    
    float variance = M[1] - M[0] * M[0];
    variance = max(variance, 0.00002);
    
    float d = refDepth - M[0];
    float p_max = variance / (variance + d * d);
    
    p_max = reduceLightBleeding(p_max, 0.5);
    
    return 1 - max(p, p_max);    
}  

float4 calcLighting(float2 uv, uint sampleIndex)
{
	float4 albedo = sampleGTex(RT0Id, uv, sampleIndex);
	float3 normal = sampleGTex(RT1Id, uv, sampleIndex).rgb;
	normal = normalize(normal * 2 - 1);

	float4 ambient = float4(0.2, 0.2, 0.2, 1);

	float NL = dot(normal, -LightDirection.xyz);
	float diffuse = saturate(NL);	

	float4 worldPos = restoreWorldPosition(uv, sampleIndex);
	worldPos = worldPos / worldPos.w;
		
	float4 shadowPos = mul(worldPos, ShadowViewProjection);
	shadowPos = shadowPos / shadowPos.w;
	float2 shadowUV = shadowPos.xy / 2 + 0.5;
	
	float bias = 0.007 * (1 - NL);
	float shadow = getShadow(shadowUV, shadowPos.z, bias);
	
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