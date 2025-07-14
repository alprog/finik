module;
#include <Windows.h>
module Timer;

uint64 startTimestamp;
uint64 cpuFrequency;
uint64 gpuFrequency;
uint64 cpuTicksInMicrosecond;

void initTimer()
{
    startTimestamp = getTimestamp();
    QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&cpuFrequency));
    cpuTicksInMicrosecond = cpuFrequency / 1'000'000;
}

uint64 getTimestamp()
{
    uint64 result;
    QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&result));
    return result;
}

uint64 get_elapsed_time()
{
    return (getTimestamp() - startTimestamp) / cpuTicksInMicrosecond;
}

uint64 getMicroseconds()
{
    return getTimestamp() / cpuTicksInMicrosecond;
}

uint64 toMicroseconds(uint64 timestamp)
{
    return timestamp / cpuTicksInMicrosecond;
}
