// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>

class Char
{
	Char() = delete;

public:
	template<class T>
	inline constexpr static bool IsWhiteSpace(const T& ch) noexcept requires
		std::same_as<T, char> ||
		std::same_as<T, wchar_t>
	{
		return ch == ' '
			|| ch == '\n'
			|| ch == '\r'
			|| ch == '\t'
			|| ch == '\b';
	}

	template<class T>
	inline constexpr static bool IsHexChar(const T& ch) noexcept requires
		std::same_as<T, char> ||
		std::same_as<T, wchar_t>
	{
		return ch >= 'a' && ch <= 'f'
			|| ch >= 'A' && ch <= 'F';
	}
};