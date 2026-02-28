// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/OSX/OSXPlatformLocalization.h"

#if PLATFORM_OSX

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/PlatformCommon.h"
#include <codecvt>
#include <locale>
#include <vector>

namespace Ayla
{
	std::wstring OSXPlatformLocalization::FromCodepage(std::string_view Str, int32 Codepage)
	{
		if (Str.empty())
		{
			return L"";
		}

		std::wstring result;
		result.reserve(Str.size());
		for (size_t i = 0; i < Str.size();)
		{
			uint32_t codepoint = 0;
			unsigned char ch = static_cast<unsigned char>(Str[i]);
			if (ch < 0x80)
			{
				codepoint = ch;
				i += 1;
			}
			else if ((ch & 0xE0) == 0xC0)
			{
				if (i + 1 >= Str.size()) break;
				codepoint = (ch & 0x1F) << 6;
				codepoint |= (static_cast<unsigned char>(Str[i + 1]) & 0x3F);
				i += 2;
			}
			else if ((ch & 0xF0) == 0xE0)
			{
				if (i + 2 >= Str.size()) break;
				codepoint = (ch & 0x0F) << 12;
				codepoint |= (static_cast<unsigned char>(Str[i + 1]) & 0x3F) << 6;
				codepoint |= (static_cast<unsigned char>(Str[i + 2]) & 0x3F);
				i += 3;
			}
			else if ((ch & 0xF8) == 0xF0)
			{
				if (i + 3 >= Str.size()) break;
				codepoint = (ch & 0x07) << 18;
				codepoint |= (static_cast<unsigned char>(Str[i + 1]) & 0x3F) << 12;
				codepoint |= (static_cast<unsigned char>(Str[i + 2]) & 0x3F) << 6;
				codepoint |= (static_cast<unsigned char>(Str[i + 3]) & 0x3F);
				i += 4;
			}
			else
			{
				i += 1;
				continue;
			}

			if constexpr (sizeof(wchar_t) == 4)
			{
				result.push_back(static_cast<wchar_t>(codepoint));
			}
			else
			{
				if (codepoint <= 0xFFFF)
				{
					result.push_back(static_cast<wchar_t>(codepoint));
				}
				else
				{
					codepoint -= 0x10000;
					result.push_back(static_cast<wchar_t>((codepoint >> 10) + 0xD800));
					result.push_back(static_cast<wchar_t>((codepoint & 0x3FF) + 0xDC00));
				}
			}
		}
		return result;
	}

	std::string OSXPlatformLocalization::ToCodepage(std::wstring_view Str, int32 Codepage)
	{
		if (Str.empty())
		{
			return "";
		}

		std::string result;
		result.reserve(Str.size() * 4);
		for (size_t i = 0; i < Str.size();)
		{
			uint32_t codepoint = 0;
			if constexpr (sizeof(wchar_t) == 4)
			{
				codepoint = static_cast<uint32_t>(Str[i]);
				i += 1;
			}
			else
			{
				uint16_t ch = static_cast<uint16_t>(Str[i]);
				if (ch >= 0xD800 && ch <= 0xDBFF)
				{
					if (i + 1 >= Str.size()) break;
					uint16_t ch2 = static_cast<uint16_t>(Str[i + 1]);
					codepoint = ((ch - 0xD800) << 10) + (ch2 - 0xDC00) + 0x10000;
					i += 2;
				}
				else
				{
					codepoint = ch;
					i += 1;
				}
			}

			if (codepoint < 0x80)
			{
				result.push_back(static_cast<char>(codepoint));
			}
			else if (codepoint < 0x800)
			{
				result.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
				result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
			}
			else if (codepoint < 0x10000)
			{
				result.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
				result.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
				result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
			}
			else
			{
				result.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
				result.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
				result.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
				result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
			}
		}
		return result;
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif
