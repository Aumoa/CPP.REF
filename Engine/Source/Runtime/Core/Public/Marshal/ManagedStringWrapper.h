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
		int32 Length;
		ssize_t IntRef;

		inline String AsString() const
		{
			return String{ C_str, (size_t)Length };
		}

		inline String AsStringView() const
		{
			return String::FromLiteral(std::wstring_view{ C_str, (size_t)Length });
		}

		static ManagedStringWrapper FromString(String str)
		{
			return ManagedStringWrapper
			{
				.C_str = str.c_str(),
				.Length = (int32)str.length(),
				.IntRef = 0
			};
		}

		static ManagedStringWrapper FromIntString(String str)
		{
			auto intRef = new String(str);
			return ManagedStringWrapper
			{
				.C_str = intRef->c_str(),
				.Length = (int32)intRef->length(),
				.IntRef = (ssize_t)intRef
			};
		}
	};
}