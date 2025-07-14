export module Containers:String;

import std;
import PrimitiveTypes;
import :Array;

export class String
{
    friend std::hash<String>;

public:
    String() = default;

    String(const char* c_string)
        : data{c_string}
    {
    }

    String(const std::string& string)
        : data{string}
    {
    }

    String(std::string&& string)
        : data{std::move(string)}
    {
    }

    String(std::initializer_list<char> list)
        : data{list}
    {
    }

    explicit String(int32 length)
        : data(length, 0)
    {
    }

    operator std::string() const
    {
        return data;
    }

    String operator +(const char* c_str) const
    {
        return data + c_str;
    }

    String operator+(const std::string& str) const
    {
        return data + str;
    }

    String operator+(const String& str) const
    {
        return data + str.data;
    }

    char& operator[](size_t index)
    {
        return data[index];
    }

    const char& operator[](size_t index) const
    {
        return data[index];
    }

    auto operator<=>(const String& other) const = default;

    auto append(char c)
    {
        data.append(&c, 1);
    }

    auto begin() const
    {
        return data.begin();
    }

    auto begin()
    {
        return data.begin();
    }

    static bool caseInsensitiveCompare(const String& lhs, const String& rhs)
    {
        return lhs.lowerCase() == rhs.lowerCase();
    }

    int32 count() const
    {
        return data.size();
    }

    auto c_str() const
    {
        return data.c_str();
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

    bool endsWith(const String& pattern)
    {
        auto stringLength = length();
        auto patternLength = pattern.length();
        if (stringLength >= patternLength)
        {
            return substr(stringLength - patternLength, patternLength) == pattern;
        }
        return false;
    }

    int32 length() const
    {
        return data.size();
    }

    String lowerCase() const
    {
        String result(data.size());
        std::transform(data.begin(), data.end(), result.begin(), [](char c) { return std::tolower(c); });
        return result;
    }

    void reserve(int32 capacity)
    {
        data.reserve(capacity);
    }

    void resize(int32 new_count)
    {
        data.resize(new_count);
    }

    int32 size() const
    {
        return data.size();
    }

    Array<String> split(String separator)
    {
        Array<String> result;

        auto separatorSize = separator.size();
        auto pattern = separator.c_str();

        std::string::size_type index = 0;
        while (index < size())
        {
            auto lastIndex = data.find(pattern, index);
            if (lastIndex == std::string::npos)
            {
                lastIndex = size();
            }

            auto subStr = substr(index, lastIndex - index);
            result.append(subStr);
            index = lastIndex + separatorSize;
        }

        return result;
    }

    bool startsWith(const String& pattern)
    {
        return substr(0, pattern.count()) == pattern;
    }

    String substr(int32 startIndex, int32 count)
    {
        return data.substr(startIndex, count);
    }

    String get_trimmed() const
    {
        constexpr const char* spacing = " \n\r\t";
        const int32 begin = data.find_first_not_of(spacing);
        if (begin == std::string::npos)
        {
            return String::Empty;
        }

        const int32 end = data.find_last_not_of(spacing);
        return data.substr(begin, end - begin + 1);
    }

    String upperCase() const
    {
        String result(data.size());
        std::transform(data.begin(), data.end(), result.begin(), [](char c) { return std::toupper(c); });
        return result;
    }

    static const String Empty;

private:
    std::string data;
};

const String String::Empty = "";

template <>
struct std::hash<String>
{
    std::size_t operator()(const String& key) const
    {
        return std::hash<std::string>()(key.data);
    }
};
