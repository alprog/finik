#pragma pack_matrix( row_major )

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

// permanent
Texture2D textures[] : register(t0, space0); // t0...
Texture2DMS<float4> texturesMS[] : register(t0, space1); // t0...

cbuffer MaterialsConstantBuffer : register(b0)
{
    Material Materials[80];
};

// per frame
cbuffer FrameConstantBuffer : register(b1)
{
	float4x4 ViewProject;
	float4x4 View;
	float4x4 Projection;	
	
	float4x4 ShadowViewProjection; // temp
	float4 LightDirection;         // temp
	uint ShadowTextureId;          // temp
};

// per draw
cbuffer MeshConstantBuffer : register(b2)
{
	float4x4 Model;
};

cbuffer MaterialConstantBuffer : register(b3)
{
    uint MaterialId; 
};

SamplerState DefaultSampler : register(s0);
SamplerState PointSampler : register(s1);
SamplerState LinearSampler : register(s2);

struct GBufferOutput
{
	float4 Albedo : SV_TARGET0;
	float4 Normals : SV_TARGET1;
};