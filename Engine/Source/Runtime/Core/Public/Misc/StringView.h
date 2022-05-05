// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "String.h"

namespace libty::inline Core
{
	class StringView : public std::wstring_view
	{
		using Super = std::wstring_view;

	private:
		std::optional<std::wstring> _conversion_impl;

	public:
		template<class... TArgs> requires
			std::constructible_from<std::wstring_view, TArgs...>
		inline StringView(TArgs&&... args) noexcept(noexcept(std::wstring_view(std::declval<TArgs>()...)))
			: Super(std::forward<TArgs>(args)...)
		{
		}

		inline StringView(const String& rhs) noexcept
			: Super(rhs)
		{
		}

		inline StringView(String& rhs) noexcept
			: Super(rhs)
		{
		}

		inline StringView(const StringView& rhs) noexcept(noexcept(std::wstring_view(std::declval<const std::wstring_view&>())))
			: Super(rhs)
		{
		}

		inline StringView(StringView&& rhs) noexcept(noexcept(std::wstring_view(std::declval<std::wstring_view&&>())))
			: Super(std::move(rhs))
		{
		}

		template<class... TArgs> requires
			std::constructible_from<std::string_view, TArgs...>
			inline StringView(TArgs&&... args) noexcept(
				noexcept(std::string_view(std::declval<TArgs>()...)) &&
				noexcept(String::AsUnicode(std::declval<std::string_view>())) &&
				noexcept(std::wstring_view(std::declval<std::wstring>()))
			)
		{
			_conversion_impl = String::AsUnicode(std::string_view(std::forward<TArgs>(args)...));
			this->std::wstring_view::operator =(*_conversion_impl);
		}

	public:
		template<class T> requires
			requires { std::declval<std::wstring_view>() = std::declval<T>(); }
		inline StringView& operator =(T&& rhs) noexcept(noexcept(std::wstring_view::operator =(std::declval<T>())))
		{
			this->std::wstring_view::operator =(std::forward<T>(rhs));
			return *this;
		}

		template<class T> requires
			std::constructible_from<std::string_view, T>
		inline StringView& operator =(T&& rhs) noexcept(
			noexcept(std::string_view(std::declval<T>())) &&
			noexcept(String::AsUnicode(std::declval<std::string_view>())) &&
			noexcept(std::wstring_view::operator =(std::declval<std::wstring>()))
			)
		{
			this->std::wstring_view::operator =(String::AsUnicode(std::string_view(std::forward<T>(rhs))));
			return *this;
		}

		inline StringView& operator =(const StringView& rhs) noexcept(noexcept(std::wstring_view::operator =(std::declval<const StringView&>())))
		{
			this->std::wstring_view::operator =(rhs);
			return *this;
		}

		inline StringView& operator =(StringView&& rhs) noexcept(noexcept(std::wstring_view::operator =(std::declval<StringView&&>())))
		{
			this->std::wstring_view::operator =(std::move(rhs));
			return *this;
		}

		template<class T> requires
			std::convertible_to<std::wstring_view, T> &&
			(!std::same_as<T, String>)
		inline operator T() const noexcept(noexcept((T)std::declval<std::wstring_view>()))
		{
			return (T)(const std::wstring_view&)*this;
		}
	};
}

inline ::libty::StringView operator "" _sv(const char* mstr, size_t len)
{
	return ::libty::StringView(std::string_view(mstr, len));
}

inline ::libty::StringView operator "" _sv(const wchar_t* wstr, size_t len)
{
	return ::libty::StringView(std::wstring_view(wstr, len));
}

inline wchar_t operator "" _sv(char mstr)
{
	return wchar_t(mstr);
}

inline wchar_t operator "" _sv(wchar_t wstr)
{
	return wstr;
}

template<>
struct std::formatter<::libty::StringView, wchar_t> : public std::formatter<std::wstring_view, wchar_t>
{
	template<class UValue, class TFormatContext>
	auto format(UValue&& value, TFormatContext& ctx)
	{
		return std::formatter<std::wstring_view, wchar_t>::format((const std::wstring_view&)value, ctx);
	}
};

#define TEXT(X) L ## X ## _sv