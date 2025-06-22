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
	return texture.Sample(PointSampler, uv);
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 currentColor = sampleTex(LightBufferId, input.uv);
	
	float2 offset = sampleTex(RT2Id, input.uv).xy;
	float2 screenOffset = float2(offset.x / 2, -offset.y / 2);
		
	float historyFactor = 0.9;

		
	float4 historyColor = sampleTex(HistoryBufferId, input.uv - screenOffset);
	
	
	return lerp(currentColor, historyColor, historyFactor);
}