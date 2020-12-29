// Copyright 2020 Aumoa.lib. All right reserved.

#include "TRefPtr.h"

#include "InvalidCastException.h"
#include "NullReferenceException.h"

void ThrowInvalidCastException()
{
	throw InvalidCastException();
}

void ThrowNullReferenceException()
{
	throw NullReferenceException();
}