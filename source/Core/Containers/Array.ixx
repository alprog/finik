export module Finik.Core.Containers:Array;

import std;
import Finik.Core.PrimitiveTypes;

export template <typename T>
class Array
{
public:
    Array() = default;

    Array(std::initializer_list<T> list)
        : data{list}
    {
    }

    T& operator[](size_t index)
    {
        return data[index];
    }

    const T& operator[](size_t index) const
    {
        return data[index];
    }

    void append(const T& value)
    {
        data.push_back(value);
    }

    void append(T&& value)
    {
        data.push_back(std::forward<T>(value));
    }

    auto begin() const
    {
        return data.begin();
    }

    auto begin()
    {
        return data.begin();
    }

    int32 capacity() const noexcept
    {
        return data.capacity();
    }

    bool contains(const T& value) const
    {
        return index_of(value) >= 0;
    }

    int32 count() const
    {
        return data.size();
    }

    void clear()
    {
        data.clear();
    }

    bool empty() const
    {
        return data.empty();
    }

    auto end() const
    {
        return data.end();
    }

    auto end()
    {
        return data.end();
    }

    template <class... Args>
    auto& emplace_back(Args&&... args)
    {
        return data.emplace_back(std::forward<Args>(args)...);
    }

    int32 index_of(const T& value) const
    {
        for (int32 i = 0; i < data.size(); i++)
        {
            if (data[i] == value)
            {
                return i;
            }
        }

        return -1;
    }

    int32 index_of(std::function<bool(const T&)> predicate)
    {
        for (int32 i = 0; i < data.size(); i++)
        {
            if (predicate(data[i]))
            {
                return i;
            }
        }

        return -1;
    }

    void insert(int32 index, const T& value)
    {
        data.insert(std::begin(data) + index, value);
    }

    void insert(int32 index, T&& value)
    {
        data.insert(std::begin(data) + index, std::forward<T>(value));
    }

    const T& last() const
    {
        return data.back();
    }

    T& last()
    {
        return data.back();
    }

    int32 last_index()
    {
        return data.size() - 1;
    }

    bool remove(const T& value)
    {
        const int32 index = index_of(value);
        if (index >= 0)
        {
            remove_at(index);
            return true;
        }
        return false;
    }

    void remove_at(int32 index)
    {
        data.erase(std::begin(data) + index);
    }

    int32 remove_if(std::function<bool(const T&)> predicate)
    {
        return std::erase_if(data, predicate);
    }

    void remove_last()
    {
        data.pop_back();
    }

    void reserve(int32 capacity)
    {
        data.reserve(capacity);
    }

    void resize(int32 new_count)
    {
        data.resize(new_count);
    }

    void resize(int32 new_count, const T& value)
    {
        data.resize(new_count, value);
    }

    void sort()
    {
        std::sort(data.begin(), data.end());
    }

    bool swap_remove(const T& value)
    {
        const int32 index = index_of(value);
        if (index >= 0)
        {
            swap_remove_at(index);
            return true;
        }
        return false;
    }

    void swap_remove_at(int32 index)
    {
        int32 last_index = count() - 1;
        if (index < last_index)
        {
            std::swap(data[index], data[last_index]);
        }
        data.pop_back();
    }

private:
    std::vector<T> data;
};
