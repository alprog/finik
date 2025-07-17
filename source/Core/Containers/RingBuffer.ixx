export module Finik.Core.Containers:RingBuffer;

import Finik.Core.PrimitiveTypes;
import std;

export template <typename T>
class RingBuffer
{
public:
    explicit RingBuffer(int32 capacity)
        : capacity{capacity}
    {
        data.reserve(capacity);
    }

    bool empty() const
    {
        return data.empty();
    }

    int32 count() const
    {
        return size;
    }

    T& operator[](int32 index)
    {
        return data[wrapIndex(headIndex - size + 1 + index)];
    }

    template <typename... Args>
    T& emplace_next(Args&&... args)
    {
        headIndex = (headIndex + 1) % capacity;

        if (size < capacity)
        {
            size++;
            return data.emplace_back(std::forward<Args>(args)...);
        }
        else
        {
            data[headIndex] = T(std::forward<Args>(args)...);
            return data[headIndex];
        }
    }

private:
    int32 wrapIndex(int32 index)
    {
        return (index + capacity) % capacity;
    }

private:
    std::vector<T> data;

    int32 headIndex = -1;
    int32 size = 0;
    int32 capacity = 0;
};
