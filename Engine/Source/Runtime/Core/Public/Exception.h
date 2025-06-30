// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "Platform/PlatformLocalization.h"
#include <exception>
#include <source_location>

namespace Ayla
{
	class CORE_API Exception : public std::exception
	{
		String m_Message;
		std::exception_ptr m_InnerException;
		std::source_location m_Src;
		String m_Stacktrace;

		mutable std::string m_WhatCache;

	public:
		Exception(const String& InMessage = TEXT("An exception was thrown."), std::exception_ptr InInnerException = {}, std::source_location Src = std::source_location::current()) noexcept;
		Exception(const Exception& Rhs) noexcept = default;
		Exception(Exception&& Rhs) noexcept = default;
		virtual ~Exception() noexcept;

		virtual String ToString() const noexcept;

		virtual String GetMessage() const noexcept { return m_Message; }
		virtual std::exception_ptr GetInnerException() const noexcept { return m_InnerException; }
		virtual std::source_location GetSourceLocation() const noexcept { return m_Src; }
		virtual String GetStackTrace() const noexcept { return m_Stacktrace; }

		virtual const char* what() const noexcept override
		{
			m_WhatCache = ToString().AsCodepage();
			return m_WhatCache.c_str();
		}
	};
}
