// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Marshal/ManagedStringWrapper.h"
#include "Marshal/NativeExceptionInterop.h"
#include "Platform/PlatformMacros.h"

namespace Ayla
{
	class CORE_API ManagedExceptionInterop
	{
	public:
		static NativeCallStatus CaptureException(
			ManagedStringWrapper typeName,
			ManagedStringWrapper message,
			ManagedStringWrapper details) noexcept;

		static void ThrowLastException();
		static void ClearLastException() noexcept;
	};
}

extern "C"
{
	PLATFORM_SHARED_EXPORT ::Ayla::NativeCallStatus Ayla__ManagedExceptionInterop__CaptureException__Injected(
		::Ayla::ManagedStringWrapper typeName,
		::Ayla::ManagedStringWrapper message,
		::Ayla::ManagedStringWrapper details) noexcept;
}
