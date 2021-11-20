// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include <Windows.h>
#include "Misc/StringUtils.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"

std::wstring StringUtils::GetPlaceholder(int32 number, std::wstring_view formatArgs)
{
	return format(L"{{{}{}}}", number, formatArgs);
}

std::wstring StringUtils::GetPlaceholder(std::wstring_view formatArgs)
{
	return format(L"{{{}}}", formatArgs);
}

std::wstring StringUtils::AsUnicode(std::string_view multibyte, uint32 codePage)
{
	if (multibyte.length() == 0)
	{
		return L"";
	}

	int32 length = MultiByteToWideChar(codePage, 0, multibyte.data(), (int32)multibyte.length(), nullptr, 0);
	if (length == 0)
	{
		LogSystem::Log(LogCore, ELogVerbosity::Error, L"Cannot convert multibyte string to unicode string with CodePage[{}].", codePage);
		return L"<ERROR_STRING_CONVERTING>";
	}

	std::wstring unicode;
	unicode.resize(length);
	MultiByteToWideChar(codePage, 0, multibyte.data(), (int32)multibyte.length(), unicode.data(), length);

	return unicode;
}

std::string StringUtils::AsMultibyte(std::wstring_view unicode, uint32 codePage)
{
	if (unicode.length() == 0)
	{
		return "";
	}

	int32 length = WideCharToMultiByte(codePage, 0, unicode.data(), (int32)unicode.length(), nullptr, 0, nullptr, nullptr);
	if (length == 0)
	{
		LogSystem::Log(LogCore, ELogVerbosity::Error, L"Cannot convert unicode string to multibyte string with CodePage[{}].", codePage);
		return "<ERROR_STRING_CONVERTING>";
	}

	std::string multibyte;
	multibyte.resize(length);
	WideCharToMultiByte(codePage, 0, unicode.data(), (int32)unicode.length(), multibyte.data(), length, nullptr, nullptr);

	return multibyte;
}

std::wstring StringUtils::Trim(const std::wstring& code, std::span<wchar_t const> trimChars)
{
	size_t trimStart = 0;
	for (size_t i = 0; i < code.length(); ++i)
	{
		if (auto it = std::find(trimChars.begin(), trimChars.end(), code[i]); it == trimChars.end())
		{
			trimStart = i;
			break;
		}
	}

	if (trimStart == code.length())
	{
		return L"";
	}
	
	size_t trimEnd = code.length();
	for (size_t i = code.length() - 1; i > trimStart; --i)
	{
		if (auto it = std::find(trimChars.begin(), trimChars.end(), code[i]); it == trimChars.end())
		{
			trimEnd = i + 1;
			break;
		}
	}

	return code.substr(trimStart, trimEnd - trimStart);
}