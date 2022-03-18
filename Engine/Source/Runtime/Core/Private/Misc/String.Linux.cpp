// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Misc/PlatformMacros.h"

#if PLATFORM_LINUX

#include "Misc/String.h"
#include <codecvt>
#include <locale>
#include <string>

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