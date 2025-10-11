// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"
#include "CharType.h"
#include "String_.h"

namespace Ayla
{
	struct ManagedStringWrapper
	{
		const char16_t* C_str;
		int32 Length;
		ssize_t IntRef;

		inline String AsString() const
		{
			return String{ C_str, (size_t)Length };
		}

		static ManagedStringWrapper FromIntString(String str)
		{
			if constexpr (sizeof(wchar_t) == 2)
			{
				auto intRef = new String(str);
				return ManagedStringWrapper
				{
					.C_str = (char16_t*)intRef->c_str(),
					.Length = (int32)intRef->length(),
					.IntRef = (ssize_t)intRef
				};
			}
			else
			{
				auto c_str = new char16_t[str.length()];
				for (size_t i = 0; i < str.length(); ++i)
				{
					c_str[i] = (char16_t)str[i];
				}
				return ManagedStringWrapper
				{
					.C_str = c_str,
					.Length = (int32)str.length(),
					.IntRef = (ssize_t)c_str
				};
			}
		}
	};
}