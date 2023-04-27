// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:CharType;

export import <concepts>;

export using char_t = wchar_t;

export struct CORE_API Char
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