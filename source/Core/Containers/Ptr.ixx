export module Finik.Core.Containers:Ptr;

import std;

export template<typename T> using Ptr = std::shared_ptr<T>;

export template <class T, class... TArgs>
Ptr<T> MakePtr(TArgs&&... args)
{
    return std::make_shared<T>(std::forward<TArgs>(args)...);
}

export template <typename T>
using UniquePtr = std::unique_ptr<T>;

export template <class T, class... TArgs>
UniquePtr<T> MakeUnique(TArgs&&... args)
{
    return std::make_unique<T>(std::forward<TArgs>(args)...);
}