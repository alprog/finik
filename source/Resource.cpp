module;
#include "asserts.h"
#include "gfx/dx.h"
module GpuResource;

import RenderSystem;

GpuResource::~GpuResource()
{
    releaseInternal();
}

void GpuResource::reinit(D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* clearValue)
{
    releaseInternal();

    auto device = Single::Get<RenderSystem>().get_device();
    device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &desc,
        initialState,
        clearValue,
        IID_PPV_ARGS(&InternalResource)) MUST;

    state = initialState;
}

void GpuResource::releaseInternal()
{
    if (InternalResource)
    {
        int32 Result = InternalResource->Release();
        ASSERT(Result == 0);
        InternalResource = nullptr;
    }
}
