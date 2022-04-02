// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <optional>
#include <string>
#include <string_view>
#include <source_location>
#include "LogVerbosity.h"

namespace libty::inline Core::inline Diagnostics
{
	class CORE_API LogCategory
	{
		friend class LogSystem;

	private:
		std::wstring CategoryName;

	public:
		LogCategory(std::wstring_view CategoryName);

		std::wstring_view GetName() const;
		static std::wstring_view VerbosityToString(ELogVerbosity Verbosity);

	protected:
		virtual void OnLog(ELogVerbosity Verbosity, std::wstring_view Message, const std::source_location& Src = std::source_location::current());
	};
}

#define DECLARE_LOG_CATEGORY(API, CategoryName) \
namespace libty::inline Generated::LogCategories \
{ \
	extern API ::libty::Core::Diagnostics::LogCategory CategoryName; \
} \

#define DEFINE_LOG_CATEGORY(CategoryName) \
namespace libty::inline Generated::LogCategories \
{ \
	::libty::Core::Diagnostics::LogCategory CategoryName(L ## #CategoryName); \
}
