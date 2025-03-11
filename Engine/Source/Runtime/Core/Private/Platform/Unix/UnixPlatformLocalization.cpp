// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "Platform/Unix/UnixPlatformLocalization.h"

#if PLATFORM_UNIX

#include "System/AssertionMacros.h"
#include <cstdlib>
#include <cerrno>

std::wstring UnixPlatformLocalization::FromCodepage(std::string_view Str, int32 InCodepage)
{
	if (!ensureMsgf(InCodepage == 0, TEXT("Codepage {} is not allowed."), InCodepage))
	{
		return L"";
	}

	int& Err = errno = 0;
	size_t NumOfCharConverted = (size_t)mbstowcs(nullptr, Str.data(), Str.length());
	if (!ensure(Err == 0))
	{
		return L"";
	}

	std::wstring Wstr;
	Wstr.resize(NumOfCharConverted);
	Err = 0;
	mbstowcs(Wstr.data(), Str.data(), Str.length());
	if (!ensure(Err == 0))
	{
		return L"";
	}

	return Wstr;
}

std::string UnixPlatformLocalization::ToCodepage(std::wstring_view Wstr, int32 InCodepage)
{
	if (!ensureMsgf(InCodepage == 0, TEXT("Codepage {} is not allowed."), InCodepage))
	{
		return "";
	}

	int& Err = errno = 0;
	size_t NumOfCharConverted = wcstombs(nullptr, Wstr.data(), Wstr.length());
	if (!ensure(Err == 0))
	{
		return "";
	}

	std::string Str;
	Str.resize(NumOfCharConverted);
	wcstombs(Str.data(), Wstr.data(), Wstr.length());
	if (!ensure(Err == 0))
	{
		return "";
	}

	return Str;
}

#endif