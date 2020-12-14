// Copyright 2020 Aumoa.lib. All right reserved.

#include "Object.h"

#include "TRefPtr.h"
#include <typeinfo>
#include "CoreString.h"

using namespace SC::Runtime::Core;

Object::Object()
	: bLockCollecting(true)
	, ref_count(0)
{

}

Object::~Object()
{

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