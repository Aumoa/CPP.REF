// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include <string>

template <typename T>
class ComPtr
{
public:
    typedef T InterfaceType;

protected:
    InterfaceType* _ptr;
    template<class U> friend class ComPtr;

    void InternalAddRef() const throw()
    {
        if (_ptr != nullptr)
        {
            _ptr->AddRef();
        }
    }

    unsigned long InternalRelease() throw()
    {
        unsigned long ref = 0;
        T* temp = _ptr;

        if (temp != nullptr)
        {
            _ptr = nullptr;
            ref = temp->Release();
        }

        return ref;
    }

public:
    ComPtr() throw() : _ptr(nullptr)
    {
    }

    ComPtr(decltype(nullptr)) throw() : _ptr(nullptr)
    {
    }

    template<class U>
    ComPtr(U* other) throw() : _ptr(other)
    {
        InternalAddRef();
    }

    ComPtr(const ComPtr& other) throw() : _ptr(other._ptr)
    {
        InternalAddRef();
    }

    template<std::derived_from<T> U>
    ComPtr(const ComPtr<U>& other) throw() :
        _ptr(other._ptr)
    {
        InternalAddRef();
    }

    ComPtr(ComPtr&& other) throw() : _ptr(nullptr)
    {
        if (this != reinterpret_cast<ComPtr*>(&reinterpret_cast<uint8&>(other)))
        {
            Swap(other);
        }
    }

    template<std::derived_from<T> U>
    ComPtr(ComPtr<U>&& other) throw() :
        _ptr(other._ptr)
    {
        other._ptr = nullptr;
    }

    ~ComPtr() throw()
    {
        InternalRelease();
    }

    ComPtr& operator=(decltype(nullptr)) throw()
    {
        InternalRelease();
        return *this;
    }

    ComPtr& operator=(T* other) throw()
    {
        if (_ptr != other)
        {
            ComPtr(other).Swap(*this);
        }
        return *this;
    }

    template <typename U>
    ComPtr& operator=(U* other) throw()
    {
        ComPtr(other).Swap(*this);
        return *this;
    }

    ComPtr& operator=(const ComPtr& other) throw()
    {
        if (_ptr != other._ptr)
        {
            ComPtr(other).Swap(*this);
        }
        return *this;
    }

    template<class U>
    ComPtr& operator=(const ComPtr<U>& other) throw()
    {
        ComPtr(other).Swap(*this);
        return *this;
    }

    ComPtr& operator=(ComPtr&& other) throw()
    {
        ComPtr(static_cast<ComPtr&&>(other)).Swap(*this);
        return *this;
    }

    template<class U>
    ComPtr& operator=(ComPtr<U>&& other) throw()
    {
        ComPtr(static_cast<ComPtr<U>&&>(other)).Swap(*this);
        return *this;
    }

    void Swap(ComPtr&& r) throw()
    {
        T* tmp = _ptr;
        _ptr = r._ptr;
        r._ptr = tmp;
    }

    void Swap(ComPtr& r) throw()
    {
        T* tmp = _ptr;
        _ptr = r._ptr;
        r._ptr = tmp;
    }

    operator bool() const throw()
    {
        return Get() != nullptr;
    }

    T* Get() const throw()
    {
        return _ptr;
    }

    InterfaceType* operator->() const throw()
    {
        return _ptr;
    }

    InterfaceType** operator&() throw()
    {
        return ReleaseAndGetAddressOf();
    }

    T* const* GetAddressOf() const throw()
    {
        return &_ptr;
    }

    T** GetAddressOf() throw()
    {
        return &_ptr;
    }

    T** ReleaseAndGetAddressOf() throw()
    {
        InternalRelease();
        return &_ptr;
    }

    T* Detach() throw()
    {
        T* ptr = _ptr;
        _ptr = nullptr;
        return ptr;
    }

    void Attach(InterfaceType* other) throw()
    {
        if (_ptr != nullptr)
        {
            _ptr->Release();
        }

        _ptr = other;
    }

    unsigned long Reset()
    {
        return InternalRelease();
    }

    bool IsSet() const
    {
        return operator bool();
    }
};