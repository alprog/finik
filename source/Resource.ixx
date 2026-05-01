export module GpuResource;

import Finik.Core;
import DX;

export class GpuResource
{
public:
    GpuResource() = default;
    virtual ~GpuResource();

    ID3D12Resource* getInternal() const
    {
        return InternalResource;
    }

protected:
    void reinit(D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* clearValue = nullptr);

private:
    void releaseInternal();

public:
    ID3D12Resource* InternalResource = nullptr;
    D3D12_RESOURCE_STATES state;
};

export class ManualGpuResource : public GpuResource
{
public:
    void reinit(D3D12_RESOURCE_DESC desc, D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* clearValue = nullptr)
    {
        GpuResource::reinit(desc, initialState, clearValue);
    }
};