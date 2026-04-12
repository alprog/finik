export module RenderEngine:Fence;

import :Shared;

export class Fence
{
public:
    Fence(CommandQueue& queue);

    uint64 SignalNext();
    uint64 GetLastSignaledValue();
    uint64 GetCompletedValue();
    void WaitForValue(uint64 value);

private:
    CommandQueue& Queue;
    MyPtr<ID3D12Fence> FenceImpl;
    HANDLE FenceEvent = nullptr;
    uint64 LastSignaledValue = 0;
};
