// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "Platform/Windows/WindowsPlatformLocalization.h"

#if PLATFORM_WINDOWS

#define __ALLOW_PLATFORM_COMMON_H__

#include "Platform/PlatformCommon.h"

std::wstring WindowsPlatformLocalization::FromCodepage(std::string_view Str, int32 Codepage)
{
	if (Str.length() == 0)
	{
		return L"";
	}

	int32 Length = MultiByteToWideChar(Codepage, 0, Str.data(), (int32)Str.length(), nullptr, 0);
	if (Length == 0)
	{
		return L"";
	}

	std::wstring Unicode;
	Unicode.resize(Length);
	MultiByteToWideChar(Codepage, 0, Str.data(), (int32)Str.length(), Unicode.data(), Length);

	return Unicode;
}

std::string WindowsPlatformLocalization::ToCodepage(std::wstring_view Str, int32 Codepage)
{
	if (Str.length() == 0)
	{
		return "";
	}

	int32 Length = WideCharToMultiByte(Codepage, 0, Str.data(), (int32)Str.length(), nullptr, 0, nullptr, nullptr);
	if (Length == 0)
	{
		return "";
	}

	std::string Multibyte;
	Multibyte.resize(Length);
	WideCharToMultiByte(Codepage, 0, Str.data(), (int32)Str.length(), Multibyte.data(), Length, nullptr, nullptr);

	return Multibyte;
}

#undef __ALLOW_PLATFORM_COMMON_H__

#endif