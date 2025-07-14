export module Allocation;

import Finik.Core;
import DX;

export namespace finik::gpumem
{
    struct RawAllocation
    {
        byte* CpuData;
        D3D12_GPU_VIRTUAL_ADDRESS GpuAddress;
    };

    template <typename TData>
    struct Allocation
    {
        Allocation(RawAllocation& raw)
            : Data { reinterpret_cast<TData*>(raw.CpuData) }
            , GpuAddress { raw.GpuAddress }
        {
        }

        TData* operator->() const throw()
        {
            return Data;
        }

        TData* Data;
        D3D12_GPU_VIRTUAL_ADDRESS GpuAddress;
    };
}
