// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"
#include "Platform/PlatformMacros.h"
#include "Marshal/ManagedStringWrapper.h"
#include <exception>

namespace Ayla
{
	enum class NativeCallStatus : int32
	{
		Success = 0,
		Exception = 1
	};

	struct CORE_API NativeExceptionInfo
	{
		ManagedStringWrapper m_TypeName;
		ManagedStringWrapper m_Message;
		ManagedStringWrapper m_Details;
		ManagedStringWrapper m_SourceFile;
		ManagedStringWrapper m_SourceFunction;
		int32 m_SourceLine;
	};

	class CORE_API NativeExceptionInterop
	{
	public:
		static NativeCallStatus CaptureCurrentException() noexcept;
		static NativeCallStatus CaptureException(std::exception_ptr exception) noexcept;
		static NativeExceptionInfo GetLastException() noexcept;
		static void ClearLastException() noexcept;
	};
}

extern "C"
{
	PLATFORM_SHARED_EXPORT ::Ayla::NativeExceptionInfo Ayla__NativeExceptionInterop__GetLastException__Injected();
	PLATFORM_SHARED_EXPORT void Ayla__NativeExceptionInterop__ClearLastException__Injected();
}
