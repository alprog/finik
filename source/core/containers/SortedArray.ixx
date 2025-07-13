export module containers:sorted_array;

import std;
import primitive_types;

export template <typename T>
class SortedArray
{
public:
    SortedArray() = default;

    SortedArray(std::initializer_list<T> list)
        : data{ list }
    {
        sort();
    }

    T& operator[](size_t index)
    {
        return data[index];
    }

    const T& operator[](size_t index) const
    {
        return data[index];
    }

    auto begin() const
    {
        return data.begin();
    }

    auto begin()
    {
        return data.begin();
    }

    int32 capacity() const
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

    int32 index_of(const T& value) const
    {
        int32 left = 0;
        int32 right = data.size() - 1;

        while (left <= right)
        {
            int32 mid = left + (right - left) / 2;
            if (data[mid] == value)
            {
                return mid;
            }
            else if (data[mid] < value)
            {
                left = mid + 1;
            }
            else
            {
                right = mid - 1;
            }
        }
        return -1;
    }

    void insert(const T& value)
    {
        auto it = std::ranges::lower_bound(data, value);
        data.insert(it, value);
    }

    const T& last() const
    {
        return data.back();
    }

    T& last()
    {
        return data.back();
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

private:
    void sort()
    {
        std::sort(data.begin(), data.end());
    }

private:
    std::vector<T> data;
};

