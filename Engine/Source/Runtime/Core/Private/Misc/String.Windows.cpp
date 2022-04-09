// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/PlatformMacros.h"

#if PLATFORM_WINDOWS

#include "Misc/String.h"
#include <Windows.h>

using namespace libty;

class string_conversion_error : public std::exception
{
	std::string _message;

public:
	template<IString<char> StringT>
	string_conversion_error(StringT&& message)
		: _message(std::forward<StringT>(message))
	{
	}

	template<IString<wchar_t> StringT>
	string_conversion_error(StringT&& message)
		: string_conversion_error(String::AsMultibyte(std::forward<StringT>(message)))
	{
	}

	virtual const char* what() const noexcept override
	{
		return _message.c_str();
	}
};

std::wstring String::AsUnicode(std::string_view multibyte, uint32 codePage)
{
	if (multibyte.length() == 0)
	{
		return L"";
	}

	int32 length = MultiByteToWideChar(codePage, 0, multibyte.data(), (int32)multibyte.length(), nullptr, 0);
	if (length == 0)
	{
		throw string_conversion_error(std::format(L"Couldn't convert to unicode string from multibyte source with CodePage '{}'", codePage));
	}

	std::wstring unicode;
	unicode.resize(length);
	MultiByteToWideChar(codePage, 0, multibyte.data(), (int32)multibyte.length(), unicode.data(), length);

	return unicode;
}

std::string String::AsMultibyte(std::wstring_view unicode, uint32 codePage)
{
	if (unicode.length() == 0)
	{
		return "";
	}

	int32 length = WideCharToMultiByte(codePage, 0, unicode.data(), (int32)unicode.length(), nullptr, 0, nullptr, nullptr);
	if (length == 0)
	{
		throw string_conversion_error(std::format(L"Couldn't convert to multibyte string from unicode source with CodePage '{}'", codePage));
	}

	std::string multibyte;
	multibyte.resize(length);
	WideCharToMultiByte(codePage, 0, unicode.data(), (int32)unicode.length(), multibyte.data(), length, nullptr, nullptr);

	return multibyte;
}

#endif