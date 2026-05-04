#include "shaders/common.inc"

// per draw
cbuffer TextureConstantBuffer : register(b4)
{
	uint BindedTextureId;
};

struct VSInput
{
	float3 position : POSITION;
	float4 uv : TEXCOORD0;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float2 uv2 : TEXCOORD2;
	float2 uv3 : TEXCOORD3;
	float2 uv4 : TEXCOORD4;
	float2 uv5 : TEXCOORD5;
	float2 uv6 : TEXCOORD6;
};

float4 screenToNDC(float3 pos)
{
	return float4(pos.x * 2 - 1, pos.y * 2 - 1, pos.z, 1);
}

PSInput BlurX(VSInput input)
{
	PSInput result;
	result.position = screenToNDC(input.position);

	float r = 2.0f / 2048.0f;
	result.uv0 = input.uv + float2(0.0, -3.0 * r);
	result.uv1 = input.uv + float2(0.0, -2.0 * r);
	result.uv2 = input.uv + float2(0.0, -1.0 * r);
	result.uv3 = input.uv;
	result.uv4 = input.uv + float2(0.0,  1.0 * r);
	result.uv5 = input.uv + float2(0.0,  2.0 * r);
	result.uv6 = input.uv + float2(0.0,  3.0 * r);
	return result;
}

PSInput BlurY(VSInput input)
{
	PSInput result;
	result.position = screenToNDC(input.position);

	float r = 2.0f / 2048.0f;
	result.uv0 = input.uv + float2(-3.0 * r, 0.0);
	result.uv1 = input.uv + float2(-2.0 * r, 0.0);
	result.uv2 = input.uv + float2(-1.0 * r, 0.0);
	result.uv3 = input.uv;
	result.uv4 = input.uv + float2( 1.0 * r, 0.0);
	result.uv5 = input.uv + float2( 2.0 * r, 0.0);
	result.uv6 = input.uv + float2( 3.0 * r, 0.0);
	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float4 color = 0;
	color += sampleTex(BindedTextureId, input.uv0) * 0.06;
	color += sampleTex(BindedTextureId, input.uv1) * 0.12;
	color += sampleTex(BindedTextureId, input.uv2) * 0.19;
	color += sampleTex(BindedTextureId, input.uv3) * 0.26;
	color += sampleTex(BindedTextureId, input.uv4) * 0.19;
	color += sampleTex(BindedTextureId, input.uv5) * 0.12;
	color += sampleTex(BindedTextureId, input.uv6) * 0.06;
	return color;
}
