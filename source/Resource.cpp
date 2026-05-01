module;
#include "asserts.h"
#include "gfx/dx.h"
module GpuResource;

import RenderSystem;

GpuResource::~GpuResource()
{
    releaseInternal();
}

void GpuResource::reinitInternalResource(D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* clearValue)
{
    releaseInternal();

    auto& device = Single::Get<RenderSystem>().engine.getDevice();
    device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &desc,
        initialState,
        clearValue,
        IID_PPV_ARGS(&internalResource)) MUST;

    state = initialState;
}

void GpuResource::transition(D3D12_RESOURCE_STATES newState, ID3D12GraphicsCommandList* commandListImpl)
{
    if (state != newState)
    {
        commandListImpl->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(internalResource, state, newState));
        state = newState;
    }
}

void GpuResource::releaseInternal(bool mustBeZero)
{
    if (internalResource)
    {
        int32 Result = internalResource->Release();
        if (mustBeZero)
        {
            ASSERT(Result == 0);
        }
        internalResource = nullptr;
    }
}
