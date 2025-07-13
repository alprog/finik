export module containers:hash_map;

import std;
import primitive_types;

export template <typename TKey, typename TValue>
class HashMap
{
    using predicate_func = std::function<bool(const std::pair<TKey, TValue>&)>;

public:
    HashMap() = default;

    HashMap(std::initializer_list<std::pair<TKey, TValue>> list)
        : data{list}
    {
    }

    const TValue& operator[](const TKey& key) const
    {
        return data[key];
    }

    TValue& operator[](const TKey& key)
    {
        return data[key];
    }

    auto begin() const
    {
        return data.begin();
    }

    auto begin()
    {
        return data.begin();
    }

    bool contains(const TKey& key) const
    {
        return data.contains(key);
    }

    bool contains(predicate_func predicate)
    {
        return std::find_if(data.begin(), data.end(), predicate) != data.end();
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

    auto find(const TKey& key) const
    {
        return data.find(key);
    }

    auto find(const TKey& key)
    {
        return data.find(key);
    }

    const TValue* find_value(const TKey& key) const
    {
        auto it = data.find(key);
        if (it != std::end(data))
        {
            return std::addressof(it->second);
        }
        return nullptr;
    }

    TValue* find_value(const TKey& key)
    {
        auto it = data.find(key);
        if (it != std::end(data))
        {
            return std::addressof(it->second);
        }
        return nullptr;
    }

    bool remove(const TKey& key)
    {
        return data.erase(key) > 0;
    }

    void remove_if(predicate_func predicate)
    {
        std::erase_if(data, predicate);
    }

private:
    std::unordered_map<TKey, TValue> data;
};