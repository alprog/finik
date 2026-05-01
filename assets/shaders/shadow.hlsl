#include "shaders/common.inc"

struct PSInput
{
	float4 position : SV_POSITION;
};

float2 PSMain(PSInput input) : SV_TARGET
{
    float depth = input.position.z; // 0..1, NDC space

    float2 moments;
    moments.x = depth;
    moments.y = depth * depth;

    float dx = ddx(depth);
	float dy = ddy(depth);
	moments.y += 0.25 * (dx * dx + dy * dy);

	return moments;
}