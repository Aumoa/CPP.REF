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
	std::wstring UCharToWString(const UChar* utf16, int32_t utf16Len)
	{
		UErrorCode status = U_ZERO_ERROR;
		int32_t utf32Len = 0;
		u_strToUTF32(nullptr, 0, &utf32Len, utf16, utf16Len, &status);
		if (status != U_BUFFER_OVERFLOW_ERROR && status != U_STRING_NOT_TERMINATED_WARNING)
		{
			return L"";
		}

		status = U_ZERO_ERROR;
		std::vector<UChar32> utf32buf(utf32Len);
		u_strToUTF32(reinterpret_cast<UChar32*>(utf32buf.data()), utf32Len, nullptr, utf16, utf16Len, &status);
		if (U_FAILURE(status))
		{
			return L"";
		}

		// wchar_t가 4바이트(UTF-32)인 경우에만 안전하게 변환
		return std::wstring(reinterpret_cast<wchar_t*>(utf32buf.data()), utf32Len);
	}

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
		utf16Str.resize((utf16Len) * sizeof(UChar));
		u_strFromUTF8(utf16Str.data(), utf16Len + 1, NULL, Str.data(), -1, &status);
		if (U_FAILURE(status))
		{
			return L"";
		}

		return UCharToWString(utf16Str.data(), static_cast<int32_t>(utf16Str.size()));
	}

	std::string LinuxPlatformLocalization::ToCodepage(std::wstring_view Str, int32 Codepage)
	{
		UErrorCode status = U_ZERO_ERROR;

		const UChar* utf16Str = reinterpret_cast<const UChar*>(Str.data());
		int32_t utf16Len = static_cast<int32_t>(Str.length());

		int32_t utf8Len = 0;
		u_strToUTF8(nullptr, 0, &utf8Len, utf16Str, utf16Len, &status);
		if (status != U_BUFFER_OVERFLOW_ERROR && status != U_STRING_NOT_TERMINATED_WARNING)
		{
			return "";
		}

		status = U_ZERO_ERROR;
		std::string utf8Str(utf8Len, '\0');  // 길이만큼 버퍼 확보
		u_strToUTF8(utf8Str.data(), utf8Len + 1, nullptr, utf16Str, utf16Len, &status);
		if (U_FAILURE(status))
		{
			return "";
		}

		return utf8Str;
	}
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif