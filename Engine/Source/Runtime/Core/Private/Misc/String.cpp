// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/String.h"

#if PLATFORM_WINDOWS

#pragma push_macro("TEXT")
#undef TEXT
#include <Windows.h>
#pragma pop_macro("TEXT")

#else

#error Not supported platform.

#endif

String& String::_Allocate_assign(const wchar_t* buf, size_t len)
{
	auto& ptr = _buf.emplace<1>(std::make_shared<wchar_t[]>(len + 1));
	memcpy(ptr.get(), buf, sizeof(wchar_t) * len);
	ptr[len] = 0;
	_len = len;
	return *this;
}

#if PLATFORM_WINDOWS

std::wstring String::_As_unicode(std::string_view s, int32 codepage)
{
	if (s.length() == 0)
	{
		return L"";
	}

	int32 length = MultiByteToWideChar(codepage, 0, s.data(), (int32)s.length(), nullptr, 0);
	if (length == 0)
	{
		return L"";
	}

	std::wstring unicode;
	unicode.resize(length);
	MultiByteToWideChar(codepage, 0, s.data(), (int32)s.length(), unicode.data(), length);

	return unicode;
}

std::string String::_As_multibyte(std::wstring_view s, int32 codepage)
{
	if (s.length() == 0)
	{
		return "";
	}

	int32 length = WideCharToMultiByte(codepage, 0, s.data(), (int32)s.length(), nullptr, 0, nullptr, nullptr);
	if (length == 0)
	{
		return "";
	}

	std::string multibyte;
	multibyte.resize(length);
	WideCharToMultiByte(codepage, 0, s.data(), (int32)s.length(), multibyte.data(), length, nullptr, nullptr);

	return multibyte;
}

#endif