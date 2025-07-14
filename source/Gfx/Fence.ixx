export module Fence;

import Myptr;
import DX;
import RenderSystemFwd;

export class Fence
{
public:
    Fence(RenderSystem& renderSystem, ID3D12CommandQueue& queue);

    int SignalNext();
    int GetLastSignaledValue();
    int GetCompletedValue();
    void WaitForValue(int value);

private:
    ID3D12CommandQueue& Queue;
    MyPtr<ID3D12Fence> FenceImpl;
    HANDLE FenceEvent = nullptr;
    int LastSignaledValue = 0;
};
