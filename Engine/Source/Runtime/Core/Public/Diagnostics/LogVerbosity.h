// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <format>

namespace Ayla
{
	enum class LogVerbosity
	{
		// Do NOT change order of this enum values.

		Verbose = 0,
		Info = 1,
		Warning = 2,
		Error = 3,
		Critical = 4
	};
}

template<class TChar> requires (std::same_as<TChar, char> || std::same_as<TChar, wchar_t>)
struct std::formatter<Ayla::LogVerbosity, TChar> : public std::formatter<Ayla::String, TChar>
{
	template<class TFormatContext>
	auto format(Ayla::LogVerbosity value, TFormatContext& ctx) const
	{
		using namespace Ayla;

		String valueStr;
		switch (value)
		{
			case LogVerbosity::Verbose:
				valueStr = TEXT("Verbose");
				break;
			case LogVerbosity::Info:
				valueStr = TEXT("Info");
				break;
			case LogVerbosity::Warning:
				valueStr = TEXT("Warning");
				break;
			case LogVerbosity::Error:
				valueStr = TEXT("Error");
				break;
			case LogVerbosity::Critical:
				valueStr = TEXT("Critical");
				break;
			default:
				valueStr = String::Format(TEXT("(LogVerbosity){}"), (int32)value);
				break;
		}

		return std::formatter<String, TChar>::format(valueStr, ctx);
	}
};
