export module Finik.Core.Containers:BinaryHeap;

import std;

export template <typename T, typename ComparatorT>
class BinaryHeap
{
public:
    int count() const 
    { 
        return data.size(); 
    }

    bool empty() const 
    {
        return data.empty(); 
    }

    void makeHeap()
    {
        std::make_heap(data.begin(), data.end(), ComparatorT());
    }

    void push(T& value)
    {
        data.push_back(value);
        std::push_heap(data.begin(), data.end(), ComparatorT());
    }

    void push(T&& value)
    {
        data.push_back(value);
        std::push_heap(data.begin(), data.end(), ComparatorT());
    }

    void pushWithoutHeapify(T& value)
    {
        data.push_back(value);
    }

    void pushWithoutHeapify(T&& value)
    {
        data.push_back(value);
    }

    T& peek()
    {
        return data.front();
    }

    T pop()
    {
        auto result = data.front();
        std::pop_heap(data.begin(), data.end(), ComparatorT());
        data.pop_back();
        return result;
    }

    void remove(T& value)
    {
        auto index = getIndexOf(value);
        if (index >= 0)
        {
            int lastIndex = data.size() - 1;
            if (index == lastIndex)
            {
                data.pop_back();
            }
            else
            {
                std::swap(data[index], data[lastIndex]);
                data.pop_back();
                heapifyUp(index);
                heapifyDown(index);
            }
        }
    }

    int getIndexOf(T& value)
    {
        int count = data.size();
        for (int i = 0; i < count; i++)
        {
            if (data[i] == value)
            {
                return i;
            }
        }
        return -1;
    }

    void heapifyUp(int index)
    {
        int parentIndex = (index - 1) / 2;
        if (parentIndex != index && isHigherPriority(index, parentIndex))
        {
            std::swap(data[index], data[parentIndex]);
            heapifyUp(parentIndex);
        }
    }

    void heapifyDown(int index)
    {
        int leftIndex = index * 2 + 1;
        if (leftIndex >= count())
        {
            return;
        }

        int rightIndex = index * 2 + 2;
        int bestChildIndex = (rightIndex < count() - 1 && isHigherPriority(rightIndex, leftIndex)) ? rightIndex : leftIndex;
        if (isHigherPriority(bestChildIndex, index))
        {
            std::swap(data[index], data[bestChildIndex]);
            heapifyDown(bestChildIndex);
        }
    }

    bool isHigherPriority(int lhsIndex, int rhsIndex)
    {
        ComparatorT comparator;
        return !comparator(data[lhsIndex], data[rhsIndex]);
    }

private:
    std::vector<T> data;
};

export template <typename T>
using BinaryHeapMax = BinaryHeap<T, std::less<>>;

export template <typename T>
using BinaryHeapMin = BinaryHeap<T, std::greater<>>;
