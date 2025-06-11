
// per draw
cbuffer MeshConstantBuffer : register(b3)
{
	float4x4 Model;
};

cbuffer MaterialConstantBuffer : register(b4)
{
    uint MaterialId; 
};