export module containers:hash_set;

import std;
import primitive_types;

export template <typename T>
class HashSet
{
public:
    HashSet() = default;

    HashSet(std::initializer_list<T> list)
        : data{ list }
    {
    }

    auto begin() const
    {
        return data.begin();
    }

    auto begin()
    {
        return data.begin();
    }

    bool contains(const T& value) const
    {
        return data.contains(value);
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

    auto insert(const T& value)
    {
        return data.insert(value);
    }

    auto insert(T&& value)
    {
        return data.insert(std::forward<T>(value));
    }

    bool remove(const T& value)
    {
        return data.erase(value) > 0;
    }

    int32 remove_if(std::function<bool(const T&)> predicate)
    {
        return std::erase_if(data, predicate);
    }

private:
    std::unordered_set<T> data;
};