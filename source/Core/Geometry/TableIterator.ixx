export module TableIterator;

import Finik.Core.Containers;

export template <typename T>
struct TableIterator
{
    TableIterator(Table<T>& table)
        : table{table}
        , index{0}
    {
    }

    Table<T>& table;
    int index;

    int x() const { return index % table.width; }
    int y() const { return index / table.width; }

    T& get()
    {
        return table.get(index);
    }

    
};