// Copyright 2020 Aumoa.lib. All right reserved.

#include "TRefPtr.h"
#include "InvalidCastException.h"

namespace SC::Runtime::Core
{
	void ThrowInvalidCastException()
	{
		throw InvalidCastException();
	}
}