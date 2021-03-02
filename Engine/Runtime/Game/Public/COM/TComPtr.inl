// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "TComPtr.h"

template<class T>
TComPtr<T>::TComPtr() : instance(nullptr)
{

}

template<class T>
TComPtr<T>::TComPtr(std::nullptr_t) : instance(nullptr)
{

}

template<class T>
TComPtr<T>::TComPtr(T* ptr) : instance(ptr)
{
	SafeAddRef();
}

template<class T>
TComPtr<T>::TComPtr(const TComPtr& ptr) : TComPtr(ptr.instance)
{

}

template<class T>
TComPtr<T>::TComPtr(TComPtr&& ptr) : instance(ptr.instance)
{
	ptr.instance = nullptr;
}

template<class T>
TComPtr<T>::~TComPtr()
{
	SafeRelease();
}

template<class T>
T* TComPtr<T>::Detach()
{
	T* ptr = instance;
	instance = nullptr;
	return ptr;
}

template<class T>
void TComPtr<T>::Attach(T* ptr)
{
	SafeRelease();
	instance = ptr;
}

template<class T>
void TComPtr<T>::Reset(T* ptr)
{
	SafeRelease();
	instance = ptr;
	SafeAddRef();
}

template<class T>
T* TComPtr<T>::Get() const
{
	return instance;
}

template<class T>
T** TComPtr<T>::GetAddressOf()
{
	return &instance;
}

template<class T>
T** TComPtr<T>::ReleaseAndGetAddressOf()
{
	SafeRelease();
	return &instance;
}

template<class T>
template<class O>
bool TComPtr<T>::Is(O** ptr) const
{
	if (O* ptr = dynamic_cast<O*>(instance); ptr != nullptr)
	{
		return true;
	}

	if (TComPtr<O> ptr; COMException::Succeeded(instance->QueryInterface(__uuidof(O), (void**)&ptr)))
	{
		return true;
	}

	return false;
}

template<class T>
template<class O>
TComPtr<O> TComPtr<T>::As() const
{
	if (O* ptr = dynamic_cast<O*>(instance); ptr != nullptr)
	{
		return ptr;
	}

	TComPtr<O> ptr;
	int32 hr = instance->QueryInterface(__uuidof(O), (void**)&ptr);
	if (COMException::Failed(hr))
	{
		throw COMException(hr);
	}

	return ptr;
}

template<class T>
template<class O>
TComPtr<O> TComPtr<T>::TryAs() const
{
	if (O* ptr = dynamic_cast<O*>(instance); ptr != nullptr)
	{
		return ptr;
	}

	if (TComPtr<O> ptr; COMException::Succeeded(instance->QueryInterface(__uuidof(O), (void**)&ptr)))
	{
		return ptr;
	}

	return nullptr;
}

template<class T>
T* TComPtr<T>::operator ->() const
{
	return Get();
}

template<class T>
TComPtr<T>::operator bool() const
{
	return IsValid;
}

template<class T>
T** TComPtr<T>::operator &()
{
	return ReleaseAndGetAddressOf();
}

template<class T>
void TComPtr<T>::SafeAddRef()
{
	if (instance != nullptr)
	{
		instance->AddRef();
	}
}

template<class T>
void TComPtr<T>::SafeRelease()
{
	if (instance != nullptr)
	{
		instance->Release();
		instance = nullptr;
	}
}