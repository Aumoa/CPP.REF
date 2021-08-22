// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string>
#include <string_view>
#include <span>
#include <format>
#include <sstream>
#include <vector>
#include "PrimitiveTypes.h"

/// <summary>
/// Provide extended functions for compose string.
/// </summary>
class CORE_API StringUtils abstract final
{
public:
	/// <summary>
	/// Join all string-convertible arguments with separator.
	/// </summary>
	/// <typeparam name="T"> Type of string convertible that implements std::formatter. </typeparam>
	/// <param name="separator"> The separator. </param>
	/// <param name="arguments"> The arguments. </param>
	/// <returns> The composed string. </returns>
	template<class T>
	static std::wstring Join(std::wstring_view separator, std::span<T const> arguments)
	{
		if (arguments.size() == 0)
		{
			return L"";
		}

		std::wstringstream wss;
		wss << std::format(L"{}", arguments[0]);
		for (size_t i = 1; i < arguments.size(); ++i)
		{
			wss << separator << std::format(L"{}", arguments[i]);
		}
		return std::move(wss).str();
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
	static std::wstring AsUnicode(const std::string& multibyte, uint32 codePage = 0);

	/// <summary>
	/// Convert unicode string to multibyte string.
	/// </summary>
	/// <param name="unicode"> The unicode string. </param>
	/// <param name="codePage"> The code page that desired converted. </param>
	/// <returns> Converted multibyte string. </returns>
	static std::string AsMultibyte(const std::wstring& unicode, uint32 codePage = 0);

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