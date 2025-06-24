#include "shaders/common.inc"

// per draw
cbuffer TAAConstantBuffer : register(b3)
{
	uint LightBufferId;
	uint HistoryBufferId;
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
	return texture.Sample(LinearSampler, uv);
}

float4 sampleGTex(uint textureId, float2 uv, uint sampleIndex)
{
	Texture2DMS<float4> texture = textures[textureId];
	float2 pos = uv * Resolution;
	return texture.Load(pos, sampleIndex);
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 currentColor = sampleTex(LightBufferId, input.uv);
	
	float2 offset = sampleGTex(RT2Id, input.uv, 1).xy;
	float2 screenOffset = float2(offset.x / 2, -offset.y / 2);
		
	float4 historyColor = sampleTex(HistoryBufferId, input.uv - screenOffset);
	
	float3 NearColor0 = sampleTex(LightBufferId, input.uv + float2(+1, 0) / Resolution).rgb;
    float3 NearColor1 = sampleTex(LightBufferId, input.uv + float2(-1, 0) / Resolution).rgb;
    float3 NearColor2 = sampleTex(LightBufferId, input.uv + float2(0, -1) / Resolution).rgb;
    float3 NearColor3 = sampleTex(LightBufferId, input.uv + float2(0, +1) / Resolution).rgb;
	float3 BoxMin = min(currentColor, min(NearColor0, min(NearColor1, min(NearColor2, NearColor3))));
    float3 BoxMax = max(currentColor, max(NearColor0, max(NearColor1, max(NearColor2, NearColor3))));
		
	float3 clamped = clamp(historyColor.rgb, BoxMin, BoxMax);

	historyColor.rgb = clamped;
	
	float historyFactor = 0.9;
	return lerp(currentColor, historyColor, historyFactor);
}

float4 PSMainDebug(PSInput input) : SV_TARGET
{
	float4 currentColor = sampleTex(LightBufferId, input.uv);
	
	float2 offset = sampleGTex(RT2Id, input.uv, 1).xy;
	float2 screenOffset = float2(offset.x / 2, -offset.y / 2);
		
	float4 historyColor = sampleTex(HistoryBufferId, input.uv - screenOffset);
	
	float3 NearColor0 = sampleTex(LightBufferId, input.uv + float2(+1, 0) / Resolution).rgb;
    float3 NearColor1 = sampleTex(LightBufferId, input.uv + float2(-1, 0) / Resolution).rgb;
    float3 NearColor2 = sampleTex(LightBufferId, input.uv + float2(0, -1) / Resolution).rgb;
    float3 NearColor3 = sampleTex(LightBufferId, input.uv + float2(0, +1) / Resolution).rgb;
	float3 BoxMin = min(currentColor, min(NearColor0, min(NearColor1, min(NearColor2, NearColor3))));
    float3 BoxMax = max(currentColor, max(NearColor0, max(NearColor1, max(NearColor2, NearColor3))));
		
	float3 clamped = clamp(historyColor.rgb, BoxMin, BoxMax);

	if (clamped.r != historyColor.r)
	{
		return 1;
	}
	else
	{
		return 0;
	}

	historyColor.rgb = clamped;
	
	float historyFactor = 0.9;
	return lerp(currentColor, historyColor, historyFactor);
}