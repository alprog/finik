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

    int32 startTimebox(CommandList& commandList, const char* label, uint8 level);
    void endTimebox(CommandList& commandList, int32 timeboxIndex);

    int32 addStamp(CommandList& commandList);
    void scheduleFrameResolve(CommandList& commandList);
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
