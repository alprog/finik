export module root_signature_params;

import core;
import surface_resolution;

// for intellisense

export struct MeshConstants
{
    Matrix Model;
    Matrix PrevModel;
};

export struct FrameConstants
{
    Matrix View;
    Matrix Projection;
    Matrix ViewProjection;
    Matrix InverseViewProjection;
    Matrix PrevViewProjection;
    Vector2 NearFar;
    Vector2 Jitter;
    Vector2 PrevJitter;
};

export struct GBufferConstants
{
    SurfaceResolution Resolution;
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

export struct TAAConstants
{
    uint32 LightBufferId;
    uint32 HistoryBufferId;
};