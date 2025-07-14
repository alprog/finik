module;
#include "dx.h"
module Fence;

import Log;
import Timer;
import RenderSystem;

Fence::Fence(RenderSystem& renderSystem, ID3D12CommandQueue& queue)
    : Queue { queue }
{
    auto result = renderSystem.get_device()->CreateFence(0, D3D12_FENCE_FLAG_NONE, __uuidof(ID3D12Fence), (void**)(&FenceImpl));
    if (FAILED(result)) throw;

    FenceEvent = CreateEvent(nullptr, 0, 0, nullptr);
    if (!FenceEvent) throw;
}

int Fence::SignalNext()
{
    Queue.Signal(FenceImpl.Get(), ++LastSignaledValue);
    return LastSignaledValue;
}

int Fence::GetLastSignaledValue()
{
    return LastSignaledValue;
}

int Fence::GetCompletedValue()
{
    return FenceImpl->GetCompletedValue();
}

void Fence::WaitForValue(int value)
{
    if (value != 0)
    {
        FenceImpl->SetEventOnCompletion(value, FenceEvent);
        WaitForSingleObject(FenceEvent, INFINITE);
    }
}
