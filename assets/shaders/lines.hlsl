#include "shaders/common.inc"
#include "shaders/geometry.inc"

struct VSInput
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PSInput VSMain(VSInput input)
{
	PSInput result;
	
	float4 worldPosition = mul(float4(input.position, 1), Model);
	result.position = mul(worldPosition, ViewProject);
	result.color = input.color;
	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return input.color;
}
