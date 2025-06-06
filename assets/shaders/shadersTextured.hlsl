#include "shaders/common.inc"

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4 uv : TEXCOORD;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float4 shadowPosition : TEXCOORD2;
	float3 normal : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

PSInput VSMain(VSInput input)
{
	PSInput result;
	
	float4 worldPosition = mul(float4(input.position, 1), Model);

	result.position = mul(worldPosition, ViewProject);
	result.shadowPosition = mul(worldPosition, ShadowViewProjection);
	result.normal = input.normal;
	result.uv = input.uv;

	return result;
}

struct GBufferOuput
{
	float4 Albedo : SV_TARGET0;
	float4 Normals : SV_TARGET1;
};

GBufferOuput PSMain(PSInput input) 
{
	GBufferOuput Out;

	//if (input.position.z == 0)
	//	return 1;

	float4 color = float4(input.normal, 1);
	color.rgb = (color.rgb + 1) / 2;
	Out.Normals = color;
	
	Texture2D mainTexture = textures[Materials[MaterialId].TextureA];
	float4 texColor = mainTexture.Sample(DefaultSampler, input.uv);
	
	float4 diffuseColor = lerp(color, texColor, 1);
	
	float diffuse = 0.2 + (dot(input.normal, -LightDirection) + 1) * 0.4;
	
	Out.Albedo = diffuse * diffuseColor;
	
	return Out;
}
