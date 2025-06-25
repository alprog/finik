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
	float4 prevPosition : TEXCOORD1;
	float4 newPosition : TEXCOORD2;
	float3 normal : TEXCOORD3;
	float2 uv : TEXCOORD4;
};

PSInput VSMain(VSInput input)
{
	PSInput result;
	
	float4 worldPosition = mul(float4(input.position, 1), Model);
	float4 prevWorldPosition = mul(float4(input.position, 1), PrevModel);

	result.position = mul(worldPosition, ViewProject);
	result.prevPosition = mul(prevWorldPosition, PrevViewProject);
	result.newPosition = mul(worldPosition, ViewProject);
	
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
		
	float4 delta = (input.newPosition / input.newPosition.w) - (input.prevPosition / input.prevPosition.w);
	delta.xy = delta.xy - Jitter + PrevJitter;
	
	Out.Motion = delta.xy;
	Out.RT4 = float4(1, 1, 0, 1);

	return Out;
}
