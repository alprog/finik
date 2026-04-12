export module Execution:Fence;

import Finik.Core.PrimitiveTypes;
import Myptr;
import DX;
import GfxDevice;

export class Fence
{
public:
    Fence(GfxDevice& gfxDevice, ID3D12CommandQueue& queue);

    uint64 SignalNext();
    uint64 GetLastSignaledValue();
    uint64 GetCompletedValue();
    void WaitForValue(uint64 value);

private:
    ID3D12CommandQueue& Queue;
    MyPtr<ID3D12Fence> FenceImpl;
    HANDLE FenceEvent = nullptr;
    uint64 LastSignaledValue = 0;
};
