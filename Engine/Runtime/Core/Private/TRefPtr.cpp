// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "TRefPtr.h"

#include "InvalidCastException.h"
#include "NullReferenceException.h"

[[noreturn]] void ThrowInvalidCastException()
{
	throw InvalidCastException();
}

[[noreturn]] void ThrowNullReferenceException()
{
	throw NullReferenceException();
}