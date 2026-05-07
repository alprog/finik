export module RenderEngine:GpuProfiler;

import :Shared;

export struct StampRange
{
    int count() const { return endIndex - startIndex; }

    int startIndex = 0;
    int endIndex = 0;
    int frameIndex = 0;
};

export enum StampType
{
    TimeboxStart,
    TimeboxEnd,
    CustomStamp
};

export struct StampInfo
{
    int32 timeboxIndex;
    StampType type;
};

export class GpuProfiler
{
public:
    GpuProfiler(RenderEngine& engine);

    int32 startTimebox(ID3D12GraphicsCommandList& commandList, const char* label);
    void endTimebox(ID3D12GraphicsCommandList& commandList, int32 timeboxIndex);

    int32 addStamp(ID3D12GraphicsCommandList& commandList);
    void scheduleFrameResolve(ID3D12GraphicsCommandList& commandList);
    void endFrameRange(int frameIndex);

    void grabReadyStamps(int completedValue);

private:
    Array<StampInfo> stampInfos;

    MyPtr<ID3D12QueryHeap> queryHeap;
    MyPtr<ID3D12Resource> readBackBuffer;

    StampRange currentRange;
    std::queue<StampRange> queue;
    uint64 ticksInMicrosecond;

    uint64 syncedGpuTimestamp;
    uint64 syncedCpuTimestamp;
    uint64 syncedCpuMicroseconds;
};
