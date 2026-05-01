export module GpuResource;

import Finik.Core;
import DX;

export class GpuResource
{
public:
    GpuResource() = default;
    virtual ~GpuResource();

    ID3D12Resource* getInternal() const { return internalResource; }

    void transition(D3D12_RESOURCE_STATES newState, ID3D12GraphicsCommandList* commandListImpl);

protected:
    void reinitInternalResource(D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* clearValue = nullptr);

    void releaseInternal();

protected:
    ID3D12Resource* internalResource = nullptr;
    D3D12_RESOURCE_STATES state;
};

export class CustomGpuResource : public GpuResource
{
public:
    void reinit(D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* clearValue = nullptr)
    {
        reinitInternalResource(desc, initialState, clearValue);
    }
};