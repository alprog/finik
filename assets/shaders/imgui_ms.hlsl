
cbuffer vertexBuffer : register(b0)
{
	float4x4 ProjectionMatrix;
};

cbuffer custom : register(b1)
{
	uint2 resolution;
	uint sampleCount;
};

struct VS_INPUT
{
	float2 pos : POSITION;
	float2 uv  : TEXCOORD0;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2DMS<float4> texture0 : register(t0);

PS_INPUT VSMain(VS_INPUT input)
{
	PS_INPUT output;	
	output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));	
	output.uv  = input.uv;
	return output;
}

float4 PSMain(PS_INPUT input) : SV_Target
{
	float2 pos = input.uv * resolution;

	float4 color = 0;
	for (int i = 0; i < sampleCount; i++)
	{
		color += texture0.Load(pos, i);
	}
	return color / sampleCount;
}