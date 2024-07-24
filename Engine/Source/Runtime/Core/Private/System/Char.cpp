// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "Platform/PlatformMacros.h"

export module Core:Char;

export import :Std;
export import :PlatformTypes;

export using char_t = PlatformTypes::char_t;
export class String;

export struct CORE_API Char
{
	char_t value;

	inline Char() noexcept
		: value(0)
	{
	}

	template<class T>
	constexpr Char(const T& rhs) noexcept requires
		std::same_as<T, char> ||
		std::same_as<T, wchar_t>
		: value(static_cast<char_t>(rhs))
	{
	}
	
	constexpr Char(const Char& rhs) noexcept
		: value(rhs.value)
	{
	}

	static constexpr FORCEINLINE bool IsWhiteSpace(char_t ch) noexcept
	{
		return ch == ' '
			|| ch == '\n'
			|| ch == '\r'
			|| ch == '\t'
			|| ch == '\b';
	}

	static constexpr FORCEINLINE bool IsHexChar(char_t ch) noexcept
	{
		return ch >= 'a' && ch <= 'f'
			|| ch >= 'A' && ch <= 'F';
	}

	static constexpr FORCEINLINE bool IsAlpha(char_t ch) noexcept
	{
		return ch >= 'a' && ch <= 'z'
			|| ch >= 'A' && ch <= 'Z';
	}

	static constexpr FORCEINLINE bool IsDigit(char_t ch) noexcept
	{
		return ch >= '0' && ch <= '9';
	}

	static constexpr FORCEINLINE bool IsHex(char_t ch) noexcept
	{
		return IsDigit(ch) || IsHexChar(ch);
	}

	static constexpr FORCEINLINE char_t ToLower(char_t ch) noexcept
	{
		return (ch >= 'A' && ch <= 'Z') ? ch - 'A' + 'a' : ch;
	}

	static constexpr FORCEINLINE char_t ToUpper(char_t ch) noexcept
	{
		return (ch >= 'a' && ch <= 'z') ? ch - 'a' + 'A' : ch;
	}

	// Defined in String.h
	static constexpr FORCEINLINE String ToStringView(const char_t& ch) noexcept;
	static FORCEINLINE String ToString(char_t ch);

	static constexpr char_t k_NullChar = 0;
	static constexpr std::array<char_t, 5> k_WhiteSpaceChars = { (char_t)' ', (char_t)'\n', (char_t)'\r', (char_t)'\t', (char_t)'\b' };
};