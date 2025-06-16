#include "shaders/common.inc"
#include "shaders/geometry.inc"

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4 uv : TEXCOORD;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float3 normal : TEXCOORD0;
	float2 uv : TEXCOORD1;
};

PSInput VSMain(VSInput input)
{
	PSInput result;
	
	float4 worldPosition = mul(float4(input.position, 1), Model);

	result.position = mul(worldPosition, ViewProject);
	result.normal = normalize(mul(input.normal, Model));
	result.uv = input.uv;

	return result;
}

GBufferOutput PSMain(PSInput input) 
{
	GBufferOutput Out;

	//if (input.position.z == 0)
	//	return 1;

	float4 color = float4(input.normal, 1);
	color.rgb = (color.rgb + 1) / 2;
	Out.Normals = color;
	
	Texture2D mainTexture = textures[Materials[MaterialId].TextureA];
	float4 texColor = mainTexture.Sample(DefaultSampler, input.uv);
	
	float4 diffuseColor = lerp(color, texColor, 1);
	
	Out.Albedo = diffuseColor;
	Out.Motion = float4(1, 0, 0, 1);
	Out.RT4 = float4(1, 1, 0, 1);

	return Out;
}
