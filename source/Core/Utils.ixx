export module Finik.Core.Utils;

import std;

export
{
    std::vector<std::string> split(std::string string, std::string separator)
    {
        std::vector<std::string> result;

        auto separatorSize = separator.size();
        auto pattern = separator.c_str();

        auto size = string.size();
        std::string::size_type index = 0;
        while (index < size)
        {
            auto lastIndex = string.find(pattern, index);
            if (lastIndex == std::string::npos)
            {
                lastIndex = size;
            }

            auto subStr = string.substr(index, lastIndex - index);
            result.push_back(subStr);
            index = lastIndex + separatorSize;
        }

        return result;
    }

    std::string join(std::vector<std::string> vector, std::string separator)
    {
        std::stringstream stream;

        auto size = vector.size();
        if (size > 0)
        {
            stream << vector[0];
            for (auto i = 1; i < size; i++)
            {
                stream << separator << vector[i];
            }
        }

        return stream.str();
    }

    void replace(std::string& string, std::string oldValue, std::string newValue)
    {
        while (true)
        {
            auto index = string.find(oldValue.c_str());
            if (index == std::string::npos)
            {
                break;
            }
            string.replace(index, oldValue.size(), newValue);
        }
    }

    std::string getReplaced(std::string string, std::string oldValue, std::string newValue)
    {
        replace(string, oldValue, newValue);
        return string;
    }

    std::string toStr(std::wstring wstr)
    {
        return std::string(wstr.begin(), wstr.end());
    }

    std::wstring toWStr(std::string str)
    {
        return std::wstring(str.begin(), str.end());
    }

    float lerp(float a, float b, float k)
    {
        return a * (1.0f - k) + b * k;
    }
}

