cbuffer vertexBuffer : register(b0)
{
	float4x4 ProjectionMatrix;
};

struct VS_INPUT
{
	float2 pos : POSITION;
	float4 col : COLOR0;
	float2 uv  : TEXCOORD0;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 uv  : TEXCOORD0;
};

Texture2DMS<float4> texture0 : register(t0);

PS_INPUT VSMain(VS_INPUT input)
{
	PS_INPUT output;	
	output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));	
	output.col = input.col;
	output.uv  = input.uv;
	return output;
}

float4 PSMain(PS_INPUT input) : SV_Target
{
	float2 pos = float2(input.uv.x * 930, input.uv.y * 550);

	float4 color = 0;
	for (int i = 0; i < 4; i++)
	{
		color += texture0.Load(pos, i);
	}
	return color / 4;
}