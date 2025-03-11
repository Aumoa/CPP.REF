// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <array>
#include <locale>
#include "Platform/PlatformMacros.h"
#include "Platform/PlatformTypes.h"

using char_t = PlatformTypes::char_t;
class String;

struct CORE_API Char
{
	char_t Value;

	inline Char() noexcept
		: Value(0)
	{
	}

	template<class T>
	constexpr Char(const T& Val) noexcept requires
		std::same_as<T, char> ||
		std::same_as<T, wchar_t>
		: Value(static_cast<char_t>(Val))
	{
	}
	
	constexpr Char(const Char& Val) noexcept
		: Value(Val.Value)
	{
	}

	static constexpr FORCEINLINE bool IsWhiteSpace(char_t Ch) noexcept
	{
		return Ch == ' '
			|| Ch == '\n'
			|| Ch == '\r'
			|| Ch == '\t'
			|| Ch == '\b';
	}

	static constexpr FORCEINLINE bool IsHexChar(char_t Ch) noexcept
	{
		return Ch >= 'a' && Ch <= 'f'
			|| Ch >= 'A' && Ch <= 'F';
	}

	static constexpr FORCEINLINE bool IsAlpha(char_t Ch) noexcept
	{
		return Ch >= 'a' && Ch <= 'z'
			|| Ch >= 'A' && Ch <= 'Z';
	}

	static constexpr FORCEINLINE bool IsDigit(char_t Ch) noexcept
	{
		return Ch >= '0' && Ch <= '9';
	}

	static constexpr FORCEINLINE bool IsHex(char_t Ch) noexcept
	{
		return IsDigit(Ch) || IsHexChar(Ch);
	}

	static constexpr FORCEINLINE char_t ToLower(char_t Ch) noexcept
	{
		return (Ch >= 'A' && Ch <= 'Z') ? Ch - 'A' + 'a' : Ch;
	}

	static constexpr FORCEINLINE char_t ToUpper(char_t Ch) noexcept
	{
		return (Ch >= 'a' && Ch <= 'z') ? Ch - 'a' + 'A' : Ch;
	}

	// Defined in String.h
	static constexpr FORCEINLINE String ToStringView(const char_t& Ch) noexcept;
	static FORCEINLINE String ToString(char_t Ch);

	static constexpr char_t NullChar = 0;
	static constexpr std::array<char_t, 5> WhiteSpaceChars = { (char_t)' ', (char_t)'\n', (char_t)'\r', (char_t)'\t', (char_t)'\b' };
};