// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string>
#include <string_view>
#include <span>
#include <format>
#include <sstream>
#include <vector>
#include <optional>
#include "PrimitiveTypes.h"
#include "Concepts/CoreConcepts.h"

/// <summary>
/// Provide extended functions for compose string.
/// </summary>
class CORE_API StringUtils
{
private:
	template<class T>
	static std::wstring ToString(const T& Arg, InheritSelector<1>&&) requires requires { std::wstring(Arg); }
	{
		return std::wstring(Arg);
	}

	template<class T>
	static const std::wstring& ToString(const T& Arg, InheritSelector<2>&&) requires std::same_as<T, std::wstring>
	{
		return Arg;
	}

	template<class T>
	static std::wstring ToString(const T& Arg, InheritSelector<0>&&)
	{
		return std::format(L"{}", Arg);
	}

	template<class T>
	static auto ToString(const T& Arg)
	{
		return ToString(Arg, InheritSelector<2>{});
	}

public:
	template<class TContainer>
	static std::wstring Join(std::wstring_view InSep, const TContainer& InArgs)
	{
		if (std::size(InArgs) == 0)
		{
			return L"";
		}

		auto It = std::begin(InArgs);

		std::wstringstream Wss;
		Wss << ToString(*It++);
		for (; It != std::end(InArgs); ++It)
		{
			Wss << InSep << ToString(*It);
		}
		return std::move(Wss).str();
	}

	/// <summary>
	/// Get placeholder text for format.
	/// </summary>
	/// <param name="number"> The argument number. </param>
	/// <param name="formatArgs"> The formatting arguments includes colon. </param>
	/// <returns> The placeholder string. </returns>
	static std::wstring GetPlaceholder(int32 number, std::wstring_view formatArgs);

	/// <summary>
	/// Get placeholder text for format.
	/// </summary>
	/// <param name="formatArgs"> The formatting arguments includes colon. </param>
	/// <returns> The placeholder string. </returns>
	static std::wstring GetPlaceholder(std::wstring_view formatArgs);

	/// <summary>
	/// Convert multibyte string to unicode string.
	/// </summary>
	/// <param name="multibyte"> The multibyte string. </param>
	/// <param name="codePage"> The code page that desired converted. </param>
	/// <returns> Converted unicode string. </returns>
	static std::wstring AsUnicode(std::string_view multibyte, uint32 codePage = 0);

	/// <summary>
	/// Convert unicode string to multibyte string.
	/// </summary>
	/// <param name="unicode"> The unicode string. </param>
	/// <param name="codePage"> The code page that desired converted. </param>
	/// <returns> Converted multibyte string. </returns>
	static std::string AsMultibyte(std::wstring_view unicode, uint32 codePage = 0);

	/// <summary>
	/// Returns a new string in which all leading and trailing occurrences of a set of specified characters from the current string are removed.
	/// </summary>
	static std::wstring Trim(const std::wstring& code, std::span<wchar_t const> trimChars);

	/// <summary>
	/// Returns a new string in which all leading and trailing occurrences of a set of specified characters from the current string are removed.
	/// </summary>
	static std::wstring Trim(const std::wstring& code)
	{
		wchar_t trimChars[] = { L' ' };
		return Trim(code, trimChars);
	}

	/// <summary>
	/// Returns a string array that contains the substrings in this instance that are delimited by elements of a specified string.
	/// </summary>
	/// <param name="formatStr"> The string to split. </param>
	/// <param name="separator"> The string that delimit the substrings in this string. </param>
	/// <param name="bRemoveEmptyEntries"> Remove empty entries. </param>
	/// <param name="bTrim"> Trim left and right strings. </param>
	template<template<class...> class TContainer = std::vector>
	static TContainer<std::wstring> Split(std::wstring_view formatStr, std::wstring_view separator, bool bRemoveEmptyEntries = false, bool bTrim = false)
	{
		TContainer<std::wstring> results;

		for (size_t i = 0; i < formatStr.length();)
		{
			std::optional<std::wstring> view;
			size_t seekp = formatStr.find(separator, i);

			if (seekp == std::wstring::npos)
			{
				// Could not found separator.
				view = formatStr.substr(i);
				i = std::wstring::npos;
			}
			else
			{
				size_t length = seekp - i;
				if (length != 0 || !bRemoveEmptyEntries)
				{
					view = formatStr.substr(i, length);
				}
				i = seekp + separator.length();
			}

			if (view)
			{
				if (bTrim)
				{
					view = Trim(view.value());
					if (bRemoveEmptyEntries && view->length() == 0)
					{
						continue;
					}
				}

				results.emplace_back(view.value());
			}
		}

		return results;
	}
};

#define WCHAR_TO_ANSI(Text, ...)	(StringUtils::AsMultibyte(Text __VA_OPT__(,) __VA_ARGS__))
#define ANSI_TO_WCHAR(Text, ...)	(StringUtils::AsUnicode(Text __VA_OPT__(,) __VA_ARGS__))

#if _UNICODE
#define WCHAR_TO_TCHAR(Text, ...)	((std::wstring)Text)
#define TCHAR_TO_WCHAR(Text, ...)	((std::wstring)Text)
#define TCHAR_TO_ANSI(Text, ...)	WCHAR_TO_ANSI(Text __VA_OPT__(,) __VA_ARGS__)
#define ANSI_TO_TCHAR(Text, ...)	ANSI_TO_WCHAR(Text __VA_OPT__(,) __VA_ARGS__)
#else
#define ANSI_TO_TCHAR(Text, ...)	((std::string)Text)
#define TCHAR_TO_ANSI(Text, ...)	((std::string)Text)
#define TCHAR_TO_WCHAR(Text, ...)	ANSI_TO_WCHAR(Text __VA_OPT__(,) __VA_ARGS__)
#define WCHAR_TO_TCHAR(Text, ...)	WCHAR_TO_ANSI(Text __VA_OPT__(,) __VA_ARGS__)
#endif