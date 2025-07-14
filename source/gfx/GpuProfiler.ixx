export module GpuProfiler;

import Finik.Core;
import RenderSystemFwd;
import DX;

export struct StampRange
{
    int count() const { return endIndex - startIndex; }

    int startIndex = 0;
    int endIndex = 0;
    int readyFenceValue = 0;
};

export class GpuProfiler
{
public:
    GpuProfiler(RenderSystem& renderSystem);

    int addStamp(ID3D12GraphicsCommandList& commandList, void* name);
    void scheduleFrameResolve(ID3D12GraphicsCommandList& commandList);
    void endFrameRange(int readyFenceValue);

    void grabReadyStamps(int completedValue);

private:
    MyPtr<ID3D12QueryHeap> queryHeap;
    MyPtr<ID3D12Resource> readBackBuffer;

    StampRange currentRange;
    std::queue<StampRange> queue;
    uint64 ticksInMicrosecond;

    uint64 syncedGpuTimestamp;
    uint64 syncedCpuTimestamp;
    uint64 syncedCpuMicroseconds;
};
