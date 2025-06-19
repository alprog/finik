
// per draw
cbuffer MeshConstantBuffer : register(b3)
{
	float4x4 Model;
	float4x4 PrevModel;
};

cbuffer MaterialConstantBuffer : register(b4)
{
    uint MaterialId; 
};