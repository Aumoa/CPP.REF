// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "NonCopyable.h"

template<class T>
class GCRoot : public NonCopyable
{
	template<class U>
	friend class GCRoot;

	T* Ptr = nullptr;

public:
	GCRoot()
	{
	}

	GCRoot(T* InPtr) : Ptr(InPtr)
	{
		SafeAddToRoot();
	}

	GCRoot(GCRoot&& InMove) : Ptr(InMove.Ptr)
	{
		InMove.Ptr = nullptr;
	}

	template<class U>
	GCRoot(GCRoot<U>&& InMove) : Ptr(dynamic_cast<T*>(InMove.Ptr))
	{
		checkf(Ptr != nullptr || InMove.Ptr == nullptr, L"U type is not convertible to T type.");
		InMove.Ptr = nullptr;
	}

	~GCRoot()
	{
		SafeRemoveFromRoot();
	}

	T* operator ->() const
	{
		return Ptr;
	}

	bool IsValid() const
	{
		return Ptr;
	}

	void Reset(T* InPtr)
	{
		if (Ptr != InPtr)
		{
			SafeRemoveFromRoot();
			Ptr = InPtr;
			SafeAddToRoot();
		}
	}

	T* Get()
	{
		return Ptr;
	}

	GCRoot& operator =(GCRoot&& InMove)
	{
		SafeRemoveFromRoot();
		Ptr = InMove.Ptr;
		InMove.Ptr = nullptr;
		return *this;
	}

	template<class U>
	GCRoot& operator =(GCRoot<U>&& InMove)
	{
		SafeRemoveFromRoot();
		Ptr = dynamic_cast<T*>(InMove.Ptr);
		check(InMove.Ptr == nullptr || Ptr);
		InMove.Ptr = nullptr;
		return *this;
	}

private:
	void SafeAddToRoot()
	{
		if (Ptr)
		{
			SObject::GC().Roots.emplace(Ptr);
		}
	}

	void SafeRemoveFromRoot()
	{
		if (Ptr)
		{
			SObject::GC().Roots.erase(Ptr);
			Ptr = nullptr;
		}
	}
};