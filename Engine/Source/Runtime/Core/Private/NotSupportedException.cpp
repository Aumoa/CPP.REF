// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "NotSupportedException.h"

namespace Ayla
{
	NotSupportedException::NotSupportedException(std::exception_ptr InInnerException)
		: Exception(TEXT("The specified function or operation is not supported."), InInnerException)
	{
	}

	NotSupportedException::~NotSupportedException() noexcept
	{
	}
}