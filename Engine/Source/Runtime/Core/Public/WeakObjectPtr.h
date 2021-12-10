// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAssert.h"

template<class T>
class WeakObjectPtr
{
	T* Ptr = nullptr;
	std::shared_ptr<bool> WeakReferences;

public:
	WeakObjectPtr()
	{
	}

	WeakObjectPtr(WeakObjectPtr&& InMove)
		: Ptr(InMove.Ptr)
		, WeakReferences(std::move(InMove.WeakReferences))
	{
		InMove.Ptr = nullptr;
	}

	WeakObjectPtr(const WeakObjectPtr& InCopy)
		: Ptr(InCopy.Ptr)
		, WeakReferences(InCopy.WeakReferences)
	{
	}

	WeakObjectPtr(T* InObject) : Ptr(InObject)
	{
		if (Ptr)
		{
			WeakReferences = Ptr->WeakReferences;
		}
	}

	bool IsValid()
	{
		if (WeakReferences)
		{
			return *WeakReferences;
		}
		else
		{
			return false;
		}
	}

	T* Get()
	{
		check(IsValid());
		return Ptr;
	}

	void Reset(T* NewPtr)
	{
		Ptr = NewPtr;
		WeakReferences = NewPtr ? NewPtr->WeakReferences : nullptr;
	}
};