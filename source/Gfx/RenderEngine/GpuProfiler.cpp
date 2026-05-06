module;
#include "../dx.h"
module RenderEngine:GpuProfiler;

import TimeboxTracker;
import Timer;
import App;
import RenderSystem;

int constexpr MAX_TIMESTAMP = 500;
int constexpr readBackRecordSize = sizeof(uint64);

GpuProfiler::GpuProfiler(RenderEngine& engine)
{
    D3D12_QUERY_HEAP_DESC queryHeapDesc = {};
    queryHeapDesc.Type = D3D12_QUERY_HEAP_TYPE_TIMESTAMP;
    queryHeapDesc.Count = MAX_TIMESTAMP;
    queryHeapDesc.NodeMask = 0;

    engine.getDevice()->CreateQueryHeap(&queryHeapDesc, IID_PPV_ARGS(&queryHeap));

    D3D12_RESOURCE_DESC bufferDesc = {};
    bufferDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    bufferDesc.Width = readBackRecordSize * MAX_TIMESTAMP; // Two timestamps
    bufferDesc.Height = 1;
    bufferDesc.DepthOrArraySize = 1;
    bufferDesc.MipLevels = 1;
    bufferDesc.Format = DXGI_FORMAT_UNKNOWN;
    bufferDesc.SampleDesc.Count = 1;
    bufferDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

    D3D12_HEAP_PROPERTIES heapProps = {};
    heapProps.Type = D3D12_HEAP_TYPE_READBACK;

    engine.getDevice()->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &bufferDesc,
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&readBackBuffer));

    uint64 gpuFrequency;
    engine.get_command_queue()->GetTimestampFrequency(&gpuFrequency);
    ticksInMicrosecond = gpuFrequency / 1'000'000;

    engine.get_command_queue()->GetClockCalibration(&syncedGpuTimestamp, &syncedCpuTimestamp);
    syncedCpuMicroseconds = toMicroseconds(syncedCpuTimestamp);

    stampInfos.resize(MAX_TIMESTAMP);
}

int32 GpuProfiler::startTimebox(ID3D12GraphicsCommandList& commandList, const char* label)
{
    int32 circularStampIndex = addStamp(commandList);

    auto& lane = App::GetInstance().profiler.GetGpuLane();
    lane.timeboxes.emplace_next(label, 4, 0, 0);
    
    auto& stampInfo = stampInfos[circularStampIndex];
    stampInfo.type = StampType::TimeboxStart;
    stampInfo.timeboxIndex = lane.timeboxes.getHeadIndex();

    return stampInfo.timeboxIndex;
}

void GpuProfiler::endTimebox(ID3D12GraphicsCommandList& commandList, int32 timeboxIndex)
{
    int32 circularStampIndex = addStamp(commandList);

    auto& stampInfo = stampInfos[circularStampIndex];
    stampInfo.type = StampType::TimeboxEnd;
    stampInfo.timeboxIndex = timeboxIndex;
}

int32 GpuProfiler::addStamp(ID3D12GraphicsCommandList& commandList)
{
    int32 index = currentRange.endIndex++ % MAX_TIMESTAMP;
    commandList.EndQuery(queryHeap.Get(), D3D12_QUERY_TYPE_TIMESTAMP, index);
    return index;
}

void GpuProfiler::scheduleFrameResolve(ID3D12GraphicsCommandList& commandList)
{
    //assert(currentRange.count() <= MAX_TIMESTAMP);

    auto resolveHelper = [&commandList, this](int start, int end)
    {
        int count = end - start;
        if (count > 0)
        {
            auto type = D3D12_QUERY_TYPE_TIMESTAMP;
            int offset = start * readBackRecordSize;
            commandList.ResolveQueryData(queryHeap.Get(), type, start, count, readBackBuffer.Get(), offset);
        }
    };

    int start = currentRange.startIndex % MAX_TIMESTAMP;
    int end = currentRange.endIndex % MAX_TIMESTAMP;
    if (start < end)
    {
        resolveHelper(start, end);
    }
    else
    {
        resolveHelper(start, MAX_TIMESTAMP);
        resolveHelper(0, end);
    }    
}

void GpuProfiler::endFrameRange(const int readyFenceValue)
{
    currentRange.readyFenceValue = readyFenceValue;
    queue.push(currentRange);
    currentRange = { currentRange.endIndex, currentRange.endIndex, 0 };
}

void GpuProfiler::grabReadyStamps(int completedValue)
{
    auto& lane = App::GetInstance().profiler.GetGpuLane();

    while (!queue.empty() && completedValue >= queue.front().readyFenceValue)
    {
        StampRange range = queue.front();
        queue.pop();

        uint32 start = range.startIndex % MAX_TIMESTAMP;
        uint32 end = range.endIndex % MAX_TIMESTAMP;

        auto helper = [this, &lane](uint32 start, uint32 end) {

            D3D12_RANGE readRange = { start * readBackRecordSize, end * readBackRecordSize };

            uint32 count = end - start;
            if (count > 0)
            {
                Array<uint64> stamps;
                stamps.resize(count);

                void* data;
                readBackBuffer->Map(0, &readRange, &data);

                uint64* stampsData = static_cast<uint64*>(data);
                std::memcpy(&stamps[0], &stampsData[start], count * readBackRecordSize);
                readBackBuffer->Unmap(0, nullptr);

                
                for (uint32 index = start; index < end; index++)
                {
                    uint64 stamp = stamps[index - start];
                    if (stamp == 0)
                    {
                        continue;
                    }

                    auto& stampInfo = stampInfos[index];
                    uint64 microseconds = (stamp - syncedGpuTimestamp) / ticksInMicrosecond + syncedCpuMicroseconds;

                    if (stampInfo.type == StampType::TimeboxStart)
                    {
                        lane.timeboxes[stampInfo.timeboxIndex].startTimestamp = microseconds;
                    }
                    else if (stampInfo.type == StampType::TimeboxEnd)
                    {
                        lane.timeboxes[stampInfo.timeboxIndex].endTimestamp = microseconds;
                    }
                }
            }
        };

        if (start < end)
        {
            helper(start, end);
        }
        else
        {
            helper(start, MAX_TIMESTAMP);
            helper(0, end);            
        }
    }
}
