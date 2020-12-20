// Copyright 2020 Aumoa.lib. All right reserved.

#include "Object.h"

#include "TRefPtr.h"
#include <typeinfo>
#include "CoreString.h"

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

using namespace SC::Runtime::Core;

Object::Object()
	: bLockCollecting(true)
	, ref_count(0)
	, weak_references(nullptr)
{

}

Object::~Object()
{
	if (weak_references != nullptr)
	{
		if (weak_references->weakReferences == 0)
		{
			delete weak_references;
			weak_references = nullptr;
		}
		else
		{
			weak_references->Invalidate();
		}
	}
}

TRefPtr<String> Object::ToString() const
{
	return typeid(*this).name();
}

bool Object::Equals(TRefPtr<Object> right) const
{
	return this == right.Get();
}

size_t Object::GetHashCode() const
{
	return (size_t)this;
}

bool Object::operator ==(const TRefPtr<Object>& right) const
{
	return this == right.Get();
}

bool Object::operator !=(const TRefPtr<Object>& right) const
{
	return this != right.Get();
}

void Object::AddRef()
{
	ref_count += 1;
}

void Object::Release()
{
	if ((ref_count -= 1) == 0)
	{
		if (!bLockCollecting)
		{
			delete this;
		}
	}
}

void Object::AddRefInterlocked()
{
	InterlockedIncrement((volatile ssize_t*)&ref_count);
}

void Object::ReleaseInterlocked()
{
	size_t decremented = InterlockedDecrement((volatile ssize_t*)&ref_count);
	if (decremented == 0)
	{
		if (!bLockCollecting)
		{
			delete this;
		}
	}
}

WeakReferences* Object::GetWeakReferences() const
{
	if (weak_references == nullptr)
	{
		weak_references = new WeakReferences();
	}
	return weak_references;
}