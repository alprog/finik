export module Myptr;

import std;

export template <typename T>
class MyPtr
{
public:
    MyPtr() throw()
        : ptr(nullptr)
    {
    }

    template <class U>
    MyPtr(U* other) throw()
        : ptr(other)
    {
        InternalAddRef();
    }

    // copy constructor
    MyPtr(const MyPtr& other) throw()
        : ptr(other.ptr)
    {
        InternalAddRef();
    }

    // move constructor
    MyPtr(MyPtr&& other) throw()
        : ptr(nullptr)
    {
        if (this != reinterpret_cast<MyPtr*>(&reinterpret_cast<unsigned char&>(other)))
        {
            Swap(other);
        }
    }

    ~MyPtr() throw()
    {
        InternalRelease();
    }

    operator bool() const throw()
    {
        return Get() != nullptr;
    }

    T* Get() const throw()
    {
        return ptr;
    }

    template <typename U>
    long As(MyPtr<U>& p)
    {
        return ptr->QueryInterface(__uuidof(U), reinterpret_cast<void**>(p.ReleaseAndGetAddressOf()));
    }

    T* Detach() throw()
    {
        T* ptr = this->ptr;
        this->ptr = nullptr;
        return ptr;
    }

    T* operator->() const throw()
    {
        return ptr;
    }

    T** operator&() throw()
    {
        return reinterpret_cast<T**>(this);
    }

    MyPtr& operator=(T* other) throw()
    {
        if (ptr != other)
        {
            MyPtr(other).Swap(*this);
        }
        return *this;
    }

    // copy assignment
    MyPtr& operator=(const MyPtr& other) throw()
    {
        if (ptr != other.ptr)
        {
            MyPtr(other).Swap(*this);
        }
        return *this;
    }

    // move assignment
    MyPtr& operator=(MyPtr&& other) throw()
    {
        MyPtr(static_cast<MyPtr&&>(other)).Swap(*this);
        return *this;
    }

    void Swap(MyPtr& r) throw()
    {
        T* tmp = ptr;
        ptr = r.ptr;
        r.ptr = tmp;
    }

    T** ReleaseAndGetAddressOf() throw()
    {
        InternalRelease();
        return &ptr;
    }

    T* const* GetAddressOf() const throw()
    {
        return &ptr;
    }

    T** GetAddressOf() throw()
    {
        return &ptr;
    }

    std::strong_ordering operator<=>(const MyPtr<T>& other) const = default;

protected:
    void InternalAddRef() const throw()
    {
        if (ptr != nullptr)
        {
            ptr->AddRef();
        }
    }

    unsigned long InternalRelease() throw()
    {
        unsigned long ref = 0;
        T* temp = ptr;

        if (temp != nullptr)
        {
            ptr = nullptr;
            ref = temp->Release();
        }

        return ref;
    }

private:
    T* ptr = nullptr;
};
