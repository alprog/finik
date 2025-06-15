export module root_signature_params;

import core;
import surface_size;

// for intellisense

export struct MeshConstants
{
    Matrix Model;
};

export struct FrameConstants
{
    Matrix View;
    Matrix Projection;
    Matrix ViewProjection;
    Matrix InverseViewProjection;
    Vector2 NearFar;
};

export struct GBufferConstants
{
    SurfaceSize SurfaceSize;
    uint32 RT0Id;
    uint32 RT1Id;
    uint32 RT2Id;
    uint32 RT3Id;
    uint32 DSId;
};

static_assert(sizeof(GBufferConstants) == 32);

export struct LightConstants
{
    Vector4 LightDirection;
    Matrix ShadowViewProjection;
    uint32 ShadowTextureId;
};