#include "shaders/common.inc"

struct PSInput
{
	float4 position : SV_POSITION;
};

float4 PSMain(PSInput input) : SV_TARGET
{
	return input.position.z;
}