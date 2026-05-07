export module Finik.Core.Containers:HistoryRingBuffer;

import Finik.Core.PrimitiveTypes;
import std;

//               head(3)
//                 V
// [ S; S+1; S+2; S+3; S-2; S-1 ] 
//   ^             ^    ^
// start          max  min

export template <typename T>
class HistoryRingBuffer
{
public:
    explicit HistoryRingBuffer(int32 capacity)
        : capacity{capacity}
    {
        data.reserve(capacity);
    }

    bool empty() const
    {
        return data.empty();
    }

    int32 storedCount() const
    {
        return data.size();
    }

    T& operator[](int32 globalIndex)
    {
        int32 localIndex = globalIndex - startGlobalIndex;
        if (localIndex >= 0)
        {
            boundCheck(localIndex <= headIndex);
        }
        else
        {
            localIndex += capacity;
            boundCheck(localIndex >= headIndex);
        }
        return data[localIndex];     
    }

    int32 getMinIndex() const
    {
        return std::max(getMaxIndex() - capacity + 1, 0);
    }

    int32 getMaxIndex() const
    {
        return startGlobalIndex + headIndex;
    }

    template <typename... Args>
    T& emplace_next(Args&&... args)
    {
        headIndex++;
        if (headIndex >= capacity)
        {
            headIndex = 0;
            startGlobalIndex += capacity;
        }

        if (data.size() < capacity)
        {
            return data.emplace_back(std::forward<Args>(args)...);
        }
        else
        {
            data[headIndex] = T(std::forward<Args>(args)...);
            return data[headIndex];
        }
    }

private:
    void boundCheck(bool condition)
    {
        if (!condition)
        {
            throw std::out_of_range("index out of valid range");
        }
    }

private:
    std::vector<T> data;

    int32 startGlobalIndex = 0;

    int32 headIndex = -1;
    int32 capacity = 0;
};
