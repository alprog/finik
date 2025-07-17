export module Finik.Core.Singleton;

import std;

export template <typename T>
class Singleton
{
public:
    static T& GetInstance()
    {
        static T instance;
        return instance;
    }
};

export class Single
{
public:
    template <typename T>
    static T& Get()
    {
        static T instance;
        return instance;
    }
};
