// Copyright 2020 Aumoa.lib. All right reserved.

#include "TRefPtr.h"

#include "InvalidCastException.h"
#include "NullReferenceException.h"

namespace SC::Runtime::Core
{
	void ThrowInvalidCastException()
	{
		throw InvalidCastException();
	}

	void ThrowNullReferenceException()
	{
		throw NullReferenceException();
	}
}