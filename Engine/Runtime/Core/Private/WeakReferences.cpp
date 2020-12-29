// Copyright 2020 Aumoa.lib. All right reserved.

#include "WeakReferences.h"

#undef interface
#include <Windows.h>

#undef InterlockedIncrement
#undef InterlockedDecrement

#ifdef _W64
#define InterlockedIncrement(x) _InterlockedIncrement64(x)
#define InterlockedDecrement(x) _InterlockedDecrement64(x)
#else
#define InterlockedIncrement(x) _InterlockedIncrement(x)
#define InterlockedDecrement(x) _InterlockedDecrement(x)
#endif

WeakReferences::WeakReferences()
	: weakReferences(0)
	, bValid(true)
{

}

WeakReferences::~WeakReferences()
{

}

void WeakReferences::AddWeakRef()
{
	++weakReferences;
}

void WeakReferences::ReleaseWeak()
{
	if (--weakReferences == 0)
	{
		if (!bValid)
		{
			delete this;
		}
	}
}

void WeakReferences::AddWeakRefInterlocked()
{
	InterlockedIncrement((volatile ssize_t*)&weakReferences);
}

void WeakReferences::ReleaseWeakInterlocked()
{
	size_t decremented = InterlockedDecrement((volatile ssize_t*)&weakReferences);
	if (decremented == 0)
	{
		if (!bValid)
		{
			delete this;
		}
	}
}

bool WeakReferences::IsValid_get() const
{
	return bValid;
}

void WeakReferences::Invalidate()
{
	bValid = false;
}