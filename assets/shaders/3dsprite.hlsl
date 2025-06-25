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

struct GBufferOutput2
{
	float4 Albedo : SV_TARGET0;
	float4 Normals : SV_TARGET1;
	float2 Motion : SV_TARGET2;
	float4 RT4 : SV_TARGET3;
	float Depth : SV_Depth;
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

GBufferOutput2 PSMain(PSInput input) 
{
	GBufferOutput2 Out;

	//if (input.position.z == 0)
	//	return 1;

	float4 color = float4(input.normal, 1);
	color.rgb = (color.rgb + 1) / 2;
	Out.Normals = color;
	
	Texture2D mainTexture = textures[Materials[MaterialId].TextureA];
	float4 texColor = mainTexture.Sample(DefaultSampler, input.uv);
	
	float a = mainTexture.Sample(DefaultSampler, input.uv + float2(+0.05, +0.05)).a;
	
	float4 diffuseColor = lerp(color, texColor, 1);
	
	Out.Albedo = diffuseColor;
	Out.Depth = input.position.z;
	
	if (diffuseColor.a < 0.5 && a > 0)
	{
		Out.Albedo = float4(1, 1, 1, a);
		Out.Depth += 0.0001f;
		Out.Normals.rgb = (float3(-1, 0, 0) + 1) / 2;
		Out.Normals.a = 1;
	}
	
	float max_a = max(diffuseColor.a, a); 
	clip (max_a - 0.5);
		
		
	float4 delta = (input.newPosition / input.newPosition.w) - (input.prevPosition / input.prevPosition.w);
	delta.xy = delta.xy - Jitter + PrevJitter;
	
	Out.Motion = delta.xy;
	Out.RT4 = float4(1, 1, 0, 1);

	

	return Out;
}
