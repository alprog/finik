#pragma pack_matrix( row_major )

#define textures ResourceDescriptorHeap

struct Material
{
    float4 ColorA;
    float4 ColorB;
    float4 ColorC;
    float4 ColorD;
    uint TextureA;
    uint TextureB;
    uint TextureC;
    uint TextureD;
};

struct GBufferOutput
{
	float4 Albedo : SV_TARGET0;
	float4 Normals : SV_TARGET1;
	float2 Motion : SV_TARGET2;
	float4 RT4 : SV_TARGET3;
	float Depth : SV_Depth;
};

cbuffer MaterialsConstantBuffer : register(b0)
{
    Material Materials[80];
};

// per frame
cbuffer FrameConstantBuffer : register(b1)
{
	float4x4 View;
	float4x4 Projection;	
	float4x4 ViewProject;
	float4x4 InverseViewProjection;
	float4x4 PrevViewProject;
	float2 NearFar;
	float2 Jitter;
	float2 PrevJitter;
};

cbuffer GBufferConstants : register(b2)
{
	uint2 Resolution;
	uint SampleCount;
	uint RT0Id;
	uint RT1Id;
	uint RT2Id;
	uint RT3Id;
	uint DSId;
};

SamplerState DefaultSampler : register(s0);
SamplerState PointSampler : register(s1);
SamplerState LinearSampler : register(s2);

float4 sampleTex(uint textureId, float2 uv, SamplerState sampler)
{
	Texture2D texture = textures[textureId];
	return texture.Sample(sampler, uv);
}

float4 sampleTex(uint textureId, float2 uv)
{
	return sampleTex(textureId, uv, LinearSampler);
}

float4 sampleGTex(uint textureId, float2 uv, uint sampleIndex)
{
	Texture2DMS<float4> texture = textures[textureId];
	float2 pos = uv * Resolution;
	return texture.Load(pos, sampleIndex);
}