// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <source_location>
#include "LogVerbosity.h"
#include "Misc/RecursiveMacroHelper.h"

namespace libty::inline Core::inline Diagnostics
{
	class CORE_API LogCategory
	{
		friend class LogSystem;

	public:
		struct Arguments
		{
			bool bLogToConsole = true;
			bool bLogToDebugger = true;
		};

	private:
		std::wstring CategoryName;
		Arguments Args;

	public:
		LogCategory(std::wstring_view CategoryName, Arguments&& InArgs);

		std::wstring_view GetName() const;
		static std::wstring_view VerbosityToString(ELogVerbosity Verbosity);

	public:
		Arguments GetArguments();

	protected:
		virtual void OnLog(ELogVerbosity Verbosity, std::wstring_view Message, const std::source_location& Src = std::source_location::current());
	};
}

#define DECLARE_LOG_CATEGORY(API, CategoryName) \
namespace libty::inline Generated::LogCategories \
{ \
	extern API ::libty::Core::Diagnostics::LogCategory CategoryName; \
} \

#define LOG_CATEGORY_ARGUMENT_FOR_EACH_ITEM(X, Value) .X = Value,

#define DEFINE_LOG_CATEGORY(CategoryName, ...) \
namespace libty::inline Generated::LogCategories \
{ \
	::libty::Core::Diagnostics::LogCategory CategoryName(L ## #CategoryName, \
		::libty::Core::Diagnostics::LogCategory::Arguments { \
			__VA_OPT__(MACRO_RECURSIVE_FOR_EACH(LOG_CATEGORY_ARGUMENT_FOR_EACH_ITEM, __VA_ARGS__)) \
		} \
	); \
}
