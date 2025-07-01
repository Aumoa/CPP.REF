// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/Linux/LinuxPlatformLocalization.h"

#if PLATFORM_LINUX

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/PlatformCommon.h"
#include <unicode/utypes.h>
#include <unicode/ustring.h>
#include <vector>

namespace Ayla
{
	std::wstring LinuxPlatformLocalization::FromCodepage(std::string_view Str, int32 Codepage)
	{
		UErrorCode status = U_ZERO_ERROR;

		int32_t utf16Len = 0;
		u_strFromUTF8(NULL, 0, &utf16Len, Str.data(), -1, &status);
		if (status != U_BUFFER_OVERFLOW_ERROR && status != U_STRING_NOT_TERMINATED_WARNING)
		{
			return L"";
		}

		status = U_ZERO_ERROR;
		std::basic_string<UChar> utf16Str;
		utf16Str.resize(utf16Len);
		u_strFromUTF8(utf16Str.data(), utf16Len + 1, NULL, Str.data(), -1, &status);
		if (U_FAILURE(status))
		{
			return L"";
		}
		
		int32_t wstrLen = 0;
		u_strToWCS(nullptr, 0, &wstrLen, utf16Str.data(), -1, &status);
		if (status != U_BUFFER_OVERFLOW_ERROR && status != U_STRING_NOT_TERMINATED_WARNING)
		{
			return L"";
		}

		status = U_ZERO_ERROR;
		std::wstring wstr;
		wstr.resize(wstrLen);
		u_strToWCS(wstr.data(), wstrLen + 1, nullptr, utf16Str.data(), -1, &status);
		if (U_FAILURE(status))
		{
			return L"";
		}

		return wstr;
	}

	std::string LinuxPlatformLocalization::ToCodepage(std::wstring_view Str, int32 Codepage)
	{
		UErrorCode status = U_ZERO_ERROR;

		int32_t ucharLen = 0;
		u_strFromWCS(nullptr, 0, &ucharLen, Str.data(), -1, &status);
		if (status != U_BUFFER_OVERFLOW_ERROR && status != U_STRING_NOT_TERMINATED_WARNING)
		{
			return "";
		}

		status = U_ZERO_ERROR;
		std::basic_string<UChar> uchar;
		uchar.resize(ucharLen);
		u_strFromWCS(uchar.data(), ucharLen + 1, nullptr, Str.data(), -1, &status);
		if (U_FAILURE(status))
		{
			return "";
		}

		int32_t utf8Len = 0;
		u_strToUTF8(nullptr, 0, &utf8Len, uchar.data(), -1, &status);
		if (status != U_BUFFER_OVERFLOW_ERROR && status != U_STRING_NOT_TERMINATED_WARNING)
		{
			return "";
		}

		status = U_ZERO_ERROR;
		std::string utf8Str;
		utf8Str.resize(utf8Len);
		u_strToUTF8(utf8Str.data(), utf8Len + 1, nullptr, uchar.data(), -1, &status);
		if (U_FAILURE(status))
		{
			return "";
		}

		return utf8Str;
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif