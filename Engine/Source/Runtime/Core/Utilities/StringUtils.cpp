// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "StringUtils.h"
#include "LogCore.h"
#include "Diagnostics/LogVerbosity.h"
#include "Diagnostics/LogSystem.h"

using namespace std;

using enum ELogVerbosity;

wstring StringUtils::GetPlaceholder(int32 number, wstring_view formatArgs)
{
	return format(L"{{{}{}}}", number, formatArgs);
}

wstring StringUtils::GetPlaceholder(wstring_view formatArgs)
{
	return format(L"{{{}}}", formatArgs);
}

wstring StringUtils::AsUnicode(const string& multibyte, uint32 codePage)
{
	int32 length = MultiByteToWideChar(codePage, 0, multibyte.c_str(), (int32)multibyte.length(), nullptr, 0);
	if (length == 0)
	{
		LogSystem::Log(LogCore, Error, L"Cannot convert multibyte string to unicode string with CodePage[{}].", codePage);
		return L"<ERROR_STRING_CONVERTING>";
	}

	wstring unicode;
	unicode.resize(length);
	MultiByteToWideChar(codePage, 0, multibyte.c_str(), (int32)multibyte.length(), unicode.data(), length);

	return unicode;
}

string StringUtils::AsMultibyte(const wstring& unicode, uint32 codePage)
{
	int32 length = WideCharToMultiByte(codePage, 0, unicode.c_str(), (int32)unicode.length(), nullptr, 0, nullptr, nullptr);
	if (length == 0)
	{
		LogSystem::Log(LogCore, Error, L"Cannot convert unicode string to multibyte string with CodePage[{}].", codePage);
		return "<ERROR_STRING_CONVERTING>";
	}

	string multibyte;
	multibyte.resize(length);
	WideCharToMultiByte(codePage, 0, unicode.c_str(), (int32)unicode.length(), multibyte.data(), length, nullptr, nullptr);

	return multibyte;
}