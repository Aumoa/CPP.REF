// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/PlatformMacros.h"
#include "Exceptions/InvalidOperationException.h"

#if PLATFORM_LINUX

#include "Misc/String.h"
#include <codecvt>
#include <locale>
#include <string>

using namespace libty;

std::wstring String::AsUnicode(std::string_view multibyte, uint32 codePage)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(std::string(multibyte));
}

std::string String::AsMultibyte(std::wstring_view unicode, uint32 codePage)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(std::wstring(unicode));
}

#endif