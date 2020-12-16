// Copyright 2020 Aumoa.lib. All right reserved.

#include "WeakReferences.h"

using namespace SC::Runtime::Core;

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
	if (--weakReferences)
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