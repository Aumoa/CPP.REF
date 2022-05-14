// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Delegates/MulticastEvent.h"
#include "LogVerbosity.h"
#include "LogCategory.h"
#include "LogVerbosity.h"
#include "Misc/String.h"
#include <exception>
#include <string>
#include <string_view>
#include <source_location>

namespace libty::inline Core
{
	class LogCategory;

	/// <summary>
	/// Provide logging functions.
	/// </summary>
	class CORE_API LogSystem
	{
		using This = LogSystem;

	public:
		template<class... TArgs>
		static void Log(const std::source_location& Location, LogCategory& Category, ELogVerbosity LogVerbosity, String Format, TArgs&&... Args)
		{
			String Message = String::Format(Format, std::forward<TArgs>(Args)...);
			InternalLog(Category, LogVerbosity, Message, Location);
		}

	private:
		static void InternalLog(LogCategory& Category, ELogVerbosity LogVerbosity, String Message, const std::source_location& Location = std::source_location::current());
	};
}

#define SE_LOG(Category, Verbosity, Format, ...) ::libty::LogSystem::Log( \
	std::source_location::current(), \
	::libty::Generated::LogCategories::Category, \
	::libty::ELogVerbosity::Verbosity, \
	Format __VA_OPT__(,) __VA_ARGS__ \
)
