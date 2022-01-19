// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <string>
#include <string_view>
#include <span>
#include <format>
#include <sstream>
#include <vector>
#include <optional>
#include <set>
#include "PrimitiveTypes.h"
#include "CoreConcepts.h"
#include "CoreAssert.h"

/// <summary>
/// Provide extended functions for compose string.
/// </summary>
class CORESOBJECT_API StringUtils
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
	template<template<class...> class StringT, class CharT, class... _Left>
	static std::basic_string<CharT> Trim(const StringT<CharT, _Left...>& code, std::span<const CharT> trimChars)
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
			if constexpr (std::same_as<CharT, char>)
			{
				return "";
			}
			else if constexpr (std::same_as<CharT, wchar_t>)
			{
				return L"";
			}
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

		return std::basic_string<CharT>(code.substr(trimStart, trimEnd - trimStart));
	}

	/// <summary>
	/// Returns a new string in which all leading and trailing occurrences of a set of specified characters from the current string are removed.
	/// </summary>
	template<template<class...> class StringT, class CharT, class... _Left>
	static std::basic_string<CharT> Trim(const StringT<CharT, _Left...>& code)
	{
		CharT trimChars[] = { (CharT)' ' };
		return Trim(code, std::span<const CharT>(trimChars));
	}

	/// <summary>
	/// Returns a string array that contains the substrings in this instance that are delimited by elements of a specified string.
	/// </summary>
	/// <param name="formatStr"> The string to split. </param>
	/// <param name="separator"> The string that delimit the substrings in this string. </param>
	/// <param name="bRemoveEmptyEntries"> Remove empty entries. </param>
	/// <param name="bTrim"> Trim left and right strings. </param>
	template<template<class...> class TContainer = std::vector, template<class...> class StringT, class CharT, class... _Left, class StringSepT>
	static TContainer<std::basic_string<CharT>> Split(const StringT<CharT, _Left...>& formatStr, const StringSepT& separator, bool bRemoveEmptyEntries = false, bool bTrim = false)
	{
		TContainer<std::basic_string<CharT>> results;

		for (size_t i = 0; i < formatStr.length();)
		{
			std::optional<std::basic_string<CharT>> view;
			size_t seekp = formatStr.find(separator, i);

			if (seekp == std::basic_string<CharT>::npos)
			{
				// Could not found separator.
				view = formatStr.substr(i);
				i = std::basic_string<CharT>::npos;
			}
			else
			{
				size_t length = seekp - i;
				if (length != 0 || !bRemoveEmptyEntries)
				{
					view = formatStr.substr(i, length);
				}
				i = seekp + std::basic_string_view<CharT>(separator).length();
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

	template<template<class...> class StringT, class CharT, class TransformF, class... _Left>
	static std::basic_string<CharT> Transform(const StringT<CharT, _Left...>& InString, TransformF&& Body)
	{
		std::basic_string<CharT> Bstr;
		Bstr.resize(InString.length());

		for (size_t i = 0; i < Bstr.length(); ++i)
		{
			Bstr[i] = (CharT)Body(InString[i]);
		}

		return Bstr;
	}

	template<template<class...> class StringT, class CharT, class... _Left>
	static std::basic_string<CharT> ToLower(const StringT<CharT, _Left...>& InString)
	{
		return Transform(InString, (int(*)(int))std::tolower);
	}

	template<template<class...> class StringT, class CharT, class... _Left>
	static std::basic_string<CharT> ToUpper(const StringT<CharT, _Left...>& InString)
	{
		return Transform(InString, (int(*)(int))std::toupper);
	}

	template<class TFind, class TReplace, template<class...> class StringT, class CharT, class... _Left>
	static std::basic_string<CharT> ReplaceAll(const StringT<CharT, _Left...>& InString, const TFind& FindStr, const TReplace& ReplaceStr)
	{
		std::basic_string<CharT> Str(InString);

		for (size_t Idx = Str.find(FindStr); Idx != std::basic_string<CharT>::npos; Idx = Str.find(FindStr, Idx + 1))
		{
			Str = Str.replace(Idx, std::basic_string_view<CharT>(FindStr).length(), std::basic_string_view<CharT>(ReplaceStr));
		}

		return Str;
	}

	template<template<class...> class StringT, class CharT, class... _Left>
	static std::basic_string<CharT> Pluralize(const StringT<CharT, _Left...>& InString, size_t Count = 2, bool bSpecial = false, bool bForceLower = true)
	{
#define MAKE_SPECIAL(X) Cast<CharT>(L ## X)
		CharT Back = *(std::end(InString) - 1);
		const bool bUpper = !bForceLower && std::isupper(Back) != 0;
		Back = (CharT)std::tolower(Back);

		if (Count <= 1 || std::basic_string_view<CharT>(InString).length() == 0 || *(std::end(InString) - 1) == 0)
		{
			return std::basic_string<CharT>(InString);
		}
		else
		{
			std::basic_string<CharT> Pluralized(InString);
			bool bAppendS = true;

			if (Pluralized.length() >= 2)
			{
				auto Tit = Pluralized.end();
				CharT Rule[2] = { *--Tit, *--Tit };
				if (Rule[0] == (CharT)'t' && Rule[1] == (CharT)'h' && bSpecial)
				{
					static std::set<std::basic_string<CharT>> Rules =
					{
						MAKE_SPECIAL("potato"),
						MAKE_SPECIAL("tomato"),
						MAKE_SPECIAL("hero"),
						MAKE_SPECIAL("cargo"),
						MAKE_SPECIAL("echo"),
						MAKE_SPECIAL("torpedo"),
						MAKE_SPECIAL("veto")
					};

					auto LowerStr = ToLower(InString);
					if (Rules.contains(LowerStr))
					{
						Pluralized += Cast<CharT>(bUpper ? L"E" : L"e");
					}
				}
				else if (Rule[0] == (CharT)'f' && Rule[1] == (CharT)'e' && bSpecial)
				{
					static std::set<std::basic_string<CharT>> Rules =
					{
						MAKE_SPECIAL("knife"),
						MAKE_SPECIAL("life"),
						MAKE_SPECIAL("wife"),
					};

					auto LowerStr = ToLower(InString);
					if (Rules.contains(LowerStr))
					{
						auto End = Pluralized.end();
						Pluralized = Pluralized.replace(End - 2, End, Cast<CharT>(bUpper ? L"VE" : L"ve"));
					}
				}
				else if (Rule[0] == (CharT)'o' && Rule[1] == (CharT)'n' && bSpecial)
				{
					static std::set<std::basic_string<CharT>> Rules =
					{
						MAKE_SPECIAL("criterion"),
						MAKE_SPECIAL("phenomenon"),
					};

					auto LowerStr = ToLower(InString);
					if (Rules.contains(LowerStr))
					{
						auto End = Pluralized.end();
						Pluralized = Pluralized.replace(End - 2, End, Cast<CharT>(bUpper ? L"A" : L"a"));
						bAppendS = false;
					}
				}
				else if (Rule[0] == (CharT)'i' && Rule[1] == (CharT)'s')
				{
					auto End = Pluralized.end();
					Pluralized = Pluralized.replace(End - 2, End, Cast<CharT>(bUpper ? L"E" : L"e"));
				}
				else if (Rule[0] == (CharT)'s' || Rule[0] == (CharT)'c' && Rule[1] == (CharT)'h')
				{
					auto End = Pluralized.end();
					Pluralized = Pluralized.replace(End - 2, End, Cast<CharT>(bUpper ? L"E" : L"e"));
				}
				else if (Rule[0] == (CharT)'u' && Rule[1] == (CharT)'s')
				{
					auto End = Pluralized.end();
					Pluralized = Pluralized.replace(End - 2, End, Cast<CharT>(bUpper ? L"E" : L"e"));
				}
				else if (Rule[1] == (CharT)'y')
				{
					auto End = Pluralized.end();
					switch (Rule[0])
					{
					case (CharT)'a':
					case (CharT)'e':
					case (CharT)'i':
					case (CharT)'o':
					case (CharT)'u':
						break;
					default:
						Pluralized = Pluralized.replace(End - 1, End, Cast<CharT>(bUpper ? L"IE" : L"ie"));
						break;
					}
				}
			}

			if (Pluralized.length() >= 1)
			{
				if (Back == (CharT)'f' && bSpecial)
				{
					static std::set<std::basic_string<CharT>> Rules =
					{
						MAKE_SPECIAL("calf"),
						MAKE_SPECIAL("leaf"),
					};

					auto LowerStr = ToLower(InString);
					if (Rules.contains(LowerStr))
					{
						auto End = Pluralized.end();
						Pluralized = Pluralized.replace(End - 1, End, Cast<CharT>(bUpper ? L"VE" : L"ve"));
					}
				}
				else if (Back == (CharT)'s' || Back == (CharT)'x' || Back == (CharT)'z')
				{
					auto End = Pluralized.end();
					Pluralized = Pluralized.replace(End - 1, End, Cast<CharT>(bUpper ? L"E" : L"e"));
				}
				else if (Back == (CharT)'y')
				{
					auto End = Pluralized.end();
					Pluralized = Pluralized.replace(End - 1, End, Cast<CharT>(bUpper ? L"IE" : L"ie"));
				}
			}

			if (bAppendS)
			{
				return Pluralized + (bUpper ? (CharT)'S' : (CharT)'s');
			}
			else
			{
				return Pluralized;
			}
		}
#undef MAKE_SPECIAL
	}

	template<class CharTTo, class StringT>
	static std::basic_string<CharTTo> Cast(const StringT& InString)
	{
		using CharT = std::remove_reference_t<std::remove_const_t<decltype(*std::begin(InString))>>;
		if constexpr (std::is_same_v<CharT, char> && std::is_same_v<CharTTo, wchar_t>)
		{
			return ANSI_TO_WCHAR(InString);
		}
		else if constexpr (std::is_same_v<CharT, wchar_t> && std::is_same_v<CharTTo, char>)
		{
			return WCHAR_TO_ANSI(InString);
		}
		else
		{
			return std::basic_string<CharTTo>(InString);
		}
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