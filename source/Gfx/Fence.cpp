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

uint64 Fence::SignalNext()
{
    Queue.Signal(FenceImpl.Get(), ++LastSignaledValue);
    return LastSignaledValue;
}

uint64 Fence::GetLastSignaledValue()
{
    return LastSignaledValue;
}

uint64 Fence::GetCompletedValue()
{
    return FenceImpl->GetCompletedValue();
}

void Fence::WaitForValue(uint64 value)
{
    if (value != 0)
    {
        FenceImpl->SetEventOnCompletion(value, FenceEvent);
        WaitForSingleObject(FenceEvent, INFINITE);
    }
}
