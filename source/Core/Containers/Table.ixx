export module Finik.Core.Containers:Table;

import Finik.Core.PrimitiveTypes;
import std;

export template <typename T>
class Table 
{
public:
    Table()
        : width{0}
        , height{0}
    {
    }

    Table(int width, int height)
    {
        resize(width, height);
    }

    void resize(int width, int height)
    {
        this->width = width;
        this->height = height;
        data.resize(width * height);
    }

    int getIndex(int x, int y)
    {
        return y * width + x;
    }

    T& get(int32 index)
    {
        return data[index];
    }

    T& get(int x, int y)
    {
        return data[y * width + x];
    }

    int count() const
    {
        return width * height;
    }

public:
    int width;
    int height;
    std::vector<T> data;
};