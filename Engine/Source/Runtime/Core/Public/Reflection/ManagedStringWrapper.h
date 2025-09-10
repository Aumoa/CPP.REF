// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"
#include "CharType.h"
#include "String_.h"

namespace Ayla
{
	struct ManagedStringWrapper
	{
		const wchar_t* C_str;
		size_t Length;

		inline String AsStringView() const
		{
			return String::FromLiteral(std::wstring_view{ C_str, Length });
		}
	};
}