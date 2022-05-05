// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "LogVerbosity.h"
#include "Misc/RecursiveMacroHelper.h"
#include "Misc/StringView.h"
#include <optional>
#include <string>
#include <string_view>
#include <source_location>

namespace libty::inline Core
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
		LogCategory(StringView CategoryName, Arguments&& InArgs);

		StringView GetName() const;
		static StringView VerbosityToString(ELogVerbosity Verbosity);

	public:
		Arguments GetArguments();

	protected:
		virtual void OnLog(ELogVerbosity Verbosity, StringView Message, const std::source_location& Src = std::source_location::current());
	};
}

#define DECLARE_LOG_CATEGORY(API, CategoryName) \
namespace libty::Generated::LogCategories \
{ \
	extern API ::libty::Core::LogCategory CategoryName; \
} \

#define LOG_CATEGORY_ARGUMENT_FOR_EACH_ITEM(X, Value) .X = Value,

#define DEFINE_LOG_CATEGORY(CategoryName, ...) \
namespace libty::Generated::LogCategories \
{ \
	::libty::Core::LogCategory CategoryName(L ## #CategoryName, \
		::libty::Core::LogCategory::Arguments { \
			__VA_OPT__(MACRO_RECURSIVE_FOR_EACH(LOG_CATEGORY_ARGUMENT_FOR_EACH_ITEM, __VA_ARGS__)) \
		} \
	); \
}
