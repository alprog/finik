#include "shaders/common.inc"
#include "shaders/geometry.inc"

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float4 uv : TEXCOORD0;
	uint2 coord : TEXCOORD1;
};

struct PSInput
{
	float4 position : SV_POSITION;
	//float4 shadowPosition : TEXCOORD3;
	float3 normal : TEXCOORD0;
	float2 uv : TEXCOORD1;
	uint2 coord : TEXCOORD2;
};

PSInput VSMain(VSInput input)
{
	PSInput result;

	float4 worldPosition = mul(float4(input.position, 1), Model);

	result.position = mul(worldPosition, ViewProject);
	//result.shadowPosition = mul(worldPosition, ShadowViewProjection);
	//result.shadowPosition /= result.shadowPosition.w;
	result.normal = input.normal;
	result.uv = input.uv;
	result.coord = input.coord;

	return result;
}

GBufferOutput PSMain(PSInput input)
{
	GBufferOutput Out;

	//sdTexture2D shadowTexture = textures[ShadowTextureId];
	
	//float2 shadowUV = float2(input.shadowPosition.x / 2 + 0.5, 0.5 - input.shadowPosition.y / 2);
	
	//float shadowValue = shadowTexture.Sample(DefaultSampler, shadowUV).r;
	//shadowValue = LinearizeDepth(shadowValue, 0.1f, 400);
	
	Out.Normals = float4(0.5, 0.5, 1, 1);
		
	Texture2D cellTexture = textures[Materials[MaterialId].TextureA];
	Texture2D gridTexture = textures[Materials[MaterialId].TextureB];
	
	Texture2DMS<float4> cellTextureMS = textures[Materials[MaterialId].TextureB];
	
	float3 fillColor = gridTexture.Sample(PointSampler, float2(input.coord) / 256).rgb;
	float3 borderColor = cellTexture.Sample(DefaultSampler, input.uv).rgb;
	
	Out.Albedo = float4(fillColor + borderColor, 1);
	Out.Motion = float2(0, 0);
	Out.RT4 = float4(1, 1, 0, 1);
	
	return Out;
}
