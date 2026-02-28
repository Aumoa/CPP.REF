// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"

namespace Ayla
{
	enum class AddressFamily
	{
		InterNetwork = 2,
		InterNetworkV6 = 23
	};
}

template<class TChar> requires (std::same_as<TChar, char> || std::same_as<TChar, wchar_t>)
struct std::formatter<Ayla::AddressFamily, TChar> : public std::formatter<Ayla::String, TChar>
{
	template<class TFormatContext>
	auto format(Ayla::AddressFamily value, TFormatContext& ctx) const
	{
		using namespace Ayla;

		String valueStr;
		switch (value)
		{
		case AddressFamily::InterNetwork:
			valueStr = TEXT("InterNetwork");
			break;
		case AddressFamily::InterNetworkV6:
			valueStr = TEXT("InterNetworkV6");
			break;
		default:
			valueStr = String::Format(TEXT("(AddressFamily){}"), (int32)value);
			break;
		}

		return std::formatter<String, TChar>::format(valueStr, ctx);
	}
};
