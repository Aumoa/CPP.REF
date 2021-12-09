// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<class T>
class WeakObjectPtr
{
	T* Ptr = nullptr;
	uint64 ObjectIndex = 0;

public:
	WeakObjectPtr()
	{
	}

	WeakObjectPtr(T* InObject) : Ptr(InObject)
	{
	}

	bool IsValid()
	{
		return true;
	}

	T* Lock()
	{
		return Ptr;
	}
};