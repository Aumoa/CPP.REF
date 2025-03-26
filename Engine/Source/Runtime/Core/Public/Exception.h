// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "Platform/PlatformLocalization.h"
#include <exception>
#include <source_location>

namespace Ayla
{
	class CORE_API Exception
	{
		String Message;
		std::exception_ptr InnerException;
		std::source_location Src;
		String Stacktrace;

	public:
		Exception(const String& InMessage = TEXT("An exception was thrown."), std::exception_ptr InInnerException = {}, std::source_location Src = std::source_location::current()) noexcept;
		Exception(const Exception& Rhs) noexcept = default;
		Exception(Exception&& Rhs) noexcept = default;
		virtual ~Exception() noexcept;

		virtual String ToString() const noexcept;

		virtual String GetMessage() const noexcept;
		virtual std::exception_ptr GetInnerException() const noexcept;
		virtual std::source_location GetSourceLocation() const noexcept;
		virtual String GetStackTrace() const noexcept;
	};
}
