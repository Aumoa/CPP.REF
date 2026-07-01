// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Marshal/ManagedExceptionInterop.h"

namespace Ayla
{
	class CORE_API ManagedCallBoundary
	{
	public:
		static void ThrowIfFailed(NativeCallStatus status)
		{
			if (status == NativeCallStatus::Succeeded)
			{
				return;
			}

			ManagedExceptionInterop::ThrowLastException();
		}
	};
}
