// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAssert.h"

template<class T>
class WeakObjectPtr
{
	struct WeakReferencePtr
	{
		int32 bValid : 1 = true;
		int32 WeakReferences : 31 = 0;
	};

	T* Ptr = nullptr;
	WeakReferencePtr* WeakReferences = nullptr;

public:
	inline WeakObjectPtr()
	{
	}

	inline WeakObjectPtr(WeakObjectPtr&& InMove)
		: Ptr(InMove.Ptr)
		, WeakReferences(std::move(InMove.WeakReferences))
	{
		InMove.Ptr = nullptr;
		InMove.WeakReferences = nullptr;
	}

	inline WeakObjectPtr(const WeakObjectPtr& InCopy)
		: Ptr(InCopy.Ptr)
		, WeakReferences(InCopy.WeakReferences)
	{
		AddWeakRef();
	}

	inline WeakObjectPtr(T* InObject) : Ptr(InObject)
	{
		if (Ptr)
		{
			WeakReferences = reinterpret_cast<WeakReferencePtr*>(Ptr->WeakReferences);
			AddWeakRef();
		}
	}

	inline ~WeakObjectPtr()
	{
		ReleaseWeakRef();
	}

	inline bool IsValid() const
	{
		return WeakReferences && WeakReferences->bValid;
	}

	inline T* Get() const
	{
		return IsValid() ? Ptr : nullptr;
	}

	inline void Reset(T* NewPtr)
	{
		ReleaseWeakRef();

		Ptr = NewPtr;
		WeakReferences = NewPtr ? reinterpret_cast<WeakReferencePtr*>(NewPtr->WeakReferences) : nullptr;

		AddWeakRef();
	}

	inline WeakObjectPtr& operator =(const WeakObjectPtr& Ptr)
	{
		ReleaseWeakRef();

		this->Ptr = Ptr.Ptr;
		this->WeakReferences = Ptr.WeakReferences;

		AddWeakRef();
		return *this;
	}

	inline WeakObjectPtr& operator =(WeakObjectPtr&& Ptr)
	{
		ReleaseWeakRef();

		this->Ptr = Ptr.Ptr;
		this->WeakReferences = Ptr.WeakReferences;

		Ptr.Ptr = nullptr;
		Ptr.WeakReferences = nullptr;
		return *this;
	}

private:
	inline void AddWeakRef()
	{
		WeakReferences && ++WeakReferences->WeakReferences;
	}

	inline void ReleaseWeakRef()
	{
		if (WeakReferences)
		{
			if (--WeakReferences->WeakReferences == 0)
			{
				delete WeakReferences;
				WeakReferences = nullptr;
			}
		}
	}
};