// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "CoreConcepts.h"
#include <vector>
#include <set>
#include <array>
#include <optional>
#include <string_view>
#include <format>

namespace libty::inline Core
{
	class CORE_API String : public std::wstring
	{
	public:
		inline String()
		{
		}

		template<class... TArgs> requires
			std::constructible_from<std::wstring, TArgs...>
		inline String(TArgs&&... args) noexcept(noexcept(std::wstring(std::declval<TArgs>()...)))
			: std::wstring(std::forward<TArgs>(args)...)
		{
		}

		inline String(const String& rhs) noexcept(noexcept(std::wstring(std::declval<const String&>())))
			: std::wstring(rhs)
		{
		}

		inline String(String&& rhs) noexcept(noexcept(std::wstring(std::declval<String&&>())))
			: std::wstring(std::move(rhs))
		{
		}

		inline String(std::string_view sv)
			: std::wstring(AsUnicode(sv))
		{
		}

		template<class... TArgs> requires
			std::constructible_from<std::string_view, TArgs...>
		inline String(TArgs&&... args)
			: String(std::string_view(std::forward<TArgs>(args)...))
		{
		}

	public:
		template<class T> requires
			requires { std::declval<std::wstring>() = std::declval<T>(); }
		inline String& operator =(T&& rhs) noexcept(noexcept(std::wstring::operator =(std::declval<T>())))
		{
			this->std::wstring::operator =(std::forward<T>(rhs));
			return *this;
		}

		template<class T> requires
			std::constructible_from<std::string_view, T>
		inline String& operator =(T&& rhs) noexcept(
			noexcept(std::string_view(std::declval<T>())) &&
			noexcept(AsUnicode(std::declval<std::string_view>())) &&
			noexcept(std::wstring::operator =(std::declval<std::wstring>()))
			)
		{
			this->std::wstring::operator =(AsUnicode(std::string_view(std::forward<T>(rhs))));
			return *this;
		}

		inline String& operator =(const String& rhs) noexcept(noexcept(std::wstring::operator =(std::declval<const String&>())))
		{
			this->std::wstring::operator =(rhs);
			return *this;
		}

		inline String& operator =(String&& rhs) noexcept(noexcept(std::wstring::operator =(std::declval<String&&>())))
		{
			this->std::wstring::operator =(std::move(rhs));
			return *this;
		}

		template<class T> requires
			requires { std::declval<std::wstring>() += std::declval<T>(); }
		inline String& operator +=(T&& rhs) noexcept(noexcept(std::wstring::operator +=(std::declval<T>())))
		{
			this->std::wstring::operator +=(std::forward<T>(rhs));
			return *this;
		}

		template<class T> requires
			std::constructible_from<std::string_view, T>
		inline String& operator +=(T&& rhs) noexcept(
			noexcept(std::string_view(std::declval<T>())) &&
			noexcept(AsUnicode(std::declval<std::string_view>())) &&
			noexcept(std::wstring::operator +=(std::declval<T>()))
			)
		{
			this->std::wstring::operator +=(AsUnicode(std::string_view(std::forward<T>(rhs))));
			return *this;
		}

		template<class T> requires
			std::convertible_to<std::wstring, T>
		inline operator T() const noexcept(noexcept((T)std::declval<std::wstring>()))
		{
			return (T)(const std::wstring&)*this;
		}

	private:
		template<class Char_t, class T>
		static auto GetTupleStringImpl(T&& arg)
		{
			std::basic_ostringstream<Char_t> oss;
			oss << std::forward<T>(arg);
			return oss.str();
		}

	public:
		template<class TFormat, class... TArgs>
		static auto Format(TFormat&& format, TArgs&&... args) requires
			IString<TFormat, StringChar_t<TFormat>>
		{
			return std::format(std::basic_string_view<StringChar_t<TFormat>>(std::forward<TFormat>(format)), std::forward<TArgs>(args)...);
		}

		template<class StringT, class SpanT, class CharT = StringChar_t<StringT>> requires
			IString<StringT, CharT>&&
			IEnumerable<SpanT, CharT>
			static std::basic_string<CharT> Trim(const StringT& sourceString, const SpanT& trimChars)
		{
			auto sourceStringView = std::basic_string_view<CharT>(sourceString);

			size_t trimStart = 0;
			for (size_t i = 0; i < sourceStringView.length(); ++i)
			{
				if (auto it = std::find(trimChars.begin(), trimChars.end(), sourceStringView[i]); it == trimChars.end())
				{
					trimStart = i;
					break;
				}
			}

			if (trimStart == sourceStringView.length())
			{
				// Emtpy string.
				return {};
			}

			size_t trimEnd = sourceStringView.length();
			for (size_t i = sourceStringView.length() - 1; i > trimStart; --i)
			{
				if (auto it = std::find(trimChars.begin(), trimChars.end(), sourceStringView[i]); it == trimChars.end())
				{
					trimEnd = i + 1;
					break;
				}
			}

			return std::basic_string<CharT>(sourceStringView.substr(trimStart, trimEnd - trimStart));
		}

		template<class StringT, class CharT = StringChar_t<StringT>> requires
			IString<StringT, CharT>
			static auto Trim(const StringT& sourceString)
		{
			return Trim(sourceString, std::array<CharT, 1>{ CharT(' ') });
		}

		template<template<class...> class TContainer = std::vector, class StringT, class CharT = StringChar_t<StringT>, class StringSepT>
		static TContainer<std::basic_string<CharT>> Split(const StringT& formatStr, const StringSepT& separator, bool bRemoveEmptyEntries = false, bool bTrim = false) requires
			IEnumerable<TContainer<CharT>, CharT>&&
			IString<StringT, CharT>&&
			IString<StringSepT, CharT>
		{
			TContainer<std::basic_string<CharT>> results;

			auto formatStrView = std::basic_string_view<CharT>(formatStr);
			auto separatorView = std::basic_string_view<CharT>(separator);

			for (size_t i = 0; i < formatStrView.length();)
			{
				std::optional<std::basic_string<CharT>> view;
				size_t seekp = formatStrView.find(separatorView, i);

				if (seekp == std::basic_string<CharT>::npos)
				{
					// Could not found separator.
					view = formatStrView.substr(i);
					i = std::basic_string<CharT>::npos;
				}
				else
				{
					size_t length = seekp - i;
					if (length != 0 || !bRemoveEmptyEntries)
					{
						view = formatStrView.substr(i, length);
					}
					i = seekp + std::basic_string_view<CharT>(separatorView).length();
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

		template<class TString, class TContainer>
		static auto Join(const TString& separator, const TContainer& args) requires
			IString<TString, StringChar_t<TString>>&&
			IEnumerable<TContainer, EnumerableItem_t<TContainer>>&&
			IString<EnumerableItem_t<TContainer>, StringChar_t<TString>>
		{
			using Char_t = StringChar_t<TString>;
			using Element_t = EnumerableItem_t<TContainer>;
			using String_t = std::basic_string<Char_t>;
			using StringView_t = std::basic_string_view<Char_t>;

			auto it = std::begin(args);
			auto end = std::end(args);
			if (end == it)
			{
				return String_t();
			}

			std::basic_stringstream<Char_t> ss;
			ss << StringView_t(*it);
			std::advance(it, 1);
			auto sepView = StringView_t(separator);

			for (; it != end; ++it)
			{
				ss << sepView << StringView_t(*it);
			}

			return std::move(ss).str();
		}

		template<class TString, class TUnaryOp>
		static auto Transform(const TString& source, TUnaryOp&& unaryOp) requires
			IString<TString, StringChar_t<TString>>&&
			ICharTransform<TUnaryOp, StringChar_t<TString>>
		{
			using Char_t = StringChar_t<TString>;
			using String_t = std::basic_string<Char_t>;
			using StringView_t = std::basic_string_view<Char_t>;

			auto sourceView = StringView_t(source);

			String_t str;
			str.resize(sourceView.length());

			for (size_t i = 0; i < sourceView.length(); ++i)
			{
				str[i] = (Char_t)unaryOp(sourceView[i]);
			}

			return str;
		}

		template<class TString>
		static auto ToLower(const TString& source) requires
			IString<TString, StringChar_t<TString>>
		{
			return Transform(source, (int(*)(int))std::tolower);
		}

		template<class TString>
		static auto ToUpper(const TString& source) requires
			IString<TString, StringChar_t<TString>>
		{
			return Transform(source, (int(*)(int))std::toupper);
		}

		template<class TTo, class TFrom>
		static TTo Cast(TFrom&& from) requires
			IString<TFrom, wchar_t>&&
			std::same_as<TTo, std::string>
		{
			return AsMultibyte(std::wstring_view(std::forward<TFrom>(from)));
		}

		template<class TTo, class TFrom>
		static TTo Cast(TFrom&& from) requires
			IString<TFrom, char>&&
			std::same_as<TTo, std::wstring>
		{
			return AsUnicode(std::string_view(std::forward<TFrom>(from)));
		}

		template<class TString>
		static auto Pluralize(const TString& source, size_t count = 2, bool isSpecial = false, bool forcedLower = true) requires
			IString<TString, StringChar_t<TString>>
		{
			using Char_t = StringChar_t<TString>;
			using String_t = std::basic_string<Char_t>;

	#define MAKE_SPECIAL(X) Cast<std::basic_string<Char_t>>(L ## X)
			Char_t back = *(std::end(source) - 1);
			const bool isUpper = !forcedLower && std::isupper(back) != 0;
			back = (Char_t)std::tolower(back);

			if (count <= 1 || std::basic_string_view<Char_t>(source).length() == 0 || *(std::end(source) - 1) == 0)
			{
				return std::basic_string<Char_t>(source);
			}
			else
			{
				String_t pluralized(source);
				bool isAppendS = true;

				if (pluralized.length() >= 2)
				{
					auto tit = pluralized.end();
					Char_t Rule[2] = { *--tit, *--tit };
					if (Rule[0] == (Char_t)'t' && Rule[1] == (Char_t)'h' && isSpecial)
					{
						static std::set<String_t> Rules =
						{
							MAKE_SPECIAL("potato"),
							MAKE_SPECIAL("tomato"),
							MAKE_SPECIAL("hero"),
							MAKE_SPECIAL("cargo"),
							MAKE_SPECIAL("echo"),
							MAKE_SPECIAL("torpedo"),
							MAKE_SPECIAL("veto")
						};

						auto LowerStr = ToLower(source);
						if (Rules.contains(LowerStr))
						{
							pluralized += Cast<std::basic_string<Char_t>>(isUpper ? L"E" : L"e");
						}
					}
					else if (Rule[0] == (Char_t)'f' && Rule[1] == (Char_t)'e' && isSpecial)
					{
						static std::set<String_t> Rules =
						{
							MAKE_SPECIAL("knife"),
							MAKE_SPECIAL("life"),
							MAKE_SPECIAL("wife"),
						};

						auto LowerStr = ToLower(source);
						if (Rules.contains(LowerStr))
						{
							auto End = pluralized.end();
							pluralized = pluralized.replace(End - 2, End, Cast<std::basic_string<Char_t>>(isUpper ? L"VE" : L"ve"));
						}
					}
					else if (Rule[0] == (Char_t)'o' && Rule[1] == (Char_t)'n' && isSpecial)
					{
						static std::set<String_t> Rules =
						{
							MAKE_SPECIAL("criterion"),
							MAKE_SPECIAL("phenomenon"),
						};

						auto LowerStr = ToLower(source);
						if (Rules.contains(LowerStr))
						{
							auto End = pluralized.end();
							pluralized = pluralized.replace(End - 2, End, Cast<std::basic_string<Char_t>>(isUpper ? L"A" : L"a"));
							isAppendS = false;
						}
					}
					else if (Rule[0] == (Char_t)'i' && Rule[1] == (Char_t)'s')
					{
						auto End = pluralized.end();
						pluralized = pluralized.replace(End - 2, End, Cast<std::basic_string<Char_t>>(isUpper ? L"E" : L"e"));
					}
					else if (Rule[0] == (Char_t)'s' || Rule[0] == (Char_t)'c' && Rule[1] == (Char_t)'h')
					{
						auto End = pluralized.end();
						pluralized = pluralized.replace(End - 2, End, Cast<std::basic_string<Char_t>>(isUpper ? L"E" : L"e"));
					}
					else if (Rule[0] == (Char_t)'u' && Rule[1] == (Char_t)'s')
					{
						auto End = pluralized.end();
						pluralized = pluralized.replace(End - 2, End, Cast<std::basic_string<Char_t>>(isUpper ? L"E" : L"e"));
					}
					else if (Rule[1] == (Char_t)'y')
					{
						auto End = pluralized.end();
						switch (Rule[0])
						{
						case (Char_t)'a':
						case (Char_t)'e':
						case (Char_t)'i':
						case (Char_t)'o':
						case (Char_t)'u':
							break;
						default:
							pluralized = pluralized.replace(End - 1, End, Cast<std::basic_string<Char_t>>(isUpper ? L"IE" : L"ie"));
							break;
						}
					}
				}

				if (pluralized.length() >= 1)
				{
					if (back == (Char_t)'f' && isSpecial)
					{
						static std::set<String_t> Rules =
						{
							MAKE_SPECIAL("calf"),
							MAKE_SPECIAL("leaf"),
						};

						auto LowerStr = ToLower(source);
						if (Rules.contains(LowerStr))
						{
							auto End = pluralized.end();
							pluralized = pluralized.replace(End - 1, End, Cast<std::basic_string<Char_t>>(isUpper ? L"VE" : L"ve"));
						}
					}
					else if (back == (Char_t)'s' || back == (Char_t)'x' || back == (Char_t)'z')
					{
						auto End = pluralized.end();
						pluralized = pluralized.replace(End - 1, End, Cast<std::basic_string<Char_t>>(isUpper ? L"E" : L"e"));
					}
					else if (back == (Char_t)'y')
					{
						auto End = pluralized.end();
						pluralized = pluralized.replace(End - 1, End, Cast<std::basic_string<Char_t>>(isUpper ? L"IE" : L"ie"));
					}
				}

				if (isAppendS)
				{
					return pluralized + (isUpper ? (Char_t)'S' : (Char_t)'s');
				}
				else
				{
					return pluralized;
				}
			}
	#undef MAKE_SPECIAL
		}

		static std::wstring GetPlaceholder(int32 number, std::wstring_view formatArgs)
		{
			if (formatArgs.empty())
			{
				return Format(L"{{{}}}", number);
			}
			else
			{
				return Format(L"{{{}:{}}}", number, formatArgs);
			}
		}

		static std::wstring GetPlaceholder(std::wstring_view formatArgs)
		{
			if (formatArgs.empty())
			{
				return L"{}";
			}
			else
			{
				return Format(L"{{:{}}}", formatArgs);
			}
		}

		template<class TFind, class TReplace, class StringT>
		static auto ReplaceAll(const StringT& InString, const TFind& FindStr, const TReplace& ReplaceStr) requires
			IString<StringT, StringChar_t<StringT>>&&
			IString<TFind, StringChar_t<TFind>>&&
			IString<TReplace, StringChar_t<TReplace>>&&
			std::same_as<StringChar_t<StringT>, StringChar_t<TFind>>&&
			std::same_as<StringChar_t<StringT>, StringChar_t<TReplace>>
		{
			using Char_t = StringChar_t<StringT>;

			std::basic_string<Char_t> Str(InString);
			std::basic_string_view<Char_t> FindStr_View(FindStr);
			std::basic_string_view<Char_t> ReplaceStr_View(ReplaceStr);

			for (size_t Idx = Str.find(FindStr); Idx != std::basic_string<Char_t>::npos; Idx = Str.find(FindStr, Idx + 1))
			{
				Str = Str.replace(Idx, FindStr_View.length(), ReplaceStr_View);
				Idx += FindStr_View.length();
			}

			return Str;
		}

		template<class StringL, class StringR>
		static bool Equals(StringL&& sl, StringR&& sr, bool ignoreCase = false) requires
			IString<StringL, StringChar_t<StringL>> &&
			IString<StringR, StringChar_t<StringR>> &&
			std::same_as<StringChar_t<StringL>, StringChar_t<StringR>>
		{
			using Char_t = StringChar_t<StringL>;
			std::basic_string_view<Char_t> sls(std::forward<StringL>(sl));
			std::basic_string_view<Char_t> srs(std::forward<StringR>(sr));

			if (sls.length() != srs.length())
			{
				return false;
			}

			for (size_t i = 0; i < sls.length(); ++i)
			{
				if (ignoreCase && std::tolower(sls[i]) != std::tolower(srs[i]))
				{
					return false;
				}
				else if (sls[i] != srs[i])
				{
					return false;
				}
			}

			return true;
		}

		// String.{Platform}.cpp
		static std::wstring AsUnicode(std::string_view source, uint32 codepage = 0);
		static std::string AsMultibyte(std::wstring_view source, uint32 codepage = 0);

		static std::wstring AsUnicode(std::wstring_view source, uint32 codepage = 0)
		{
			return std::wstring(source);
		}

		static std::string AsMultibyte(std::string_view source, uint32 codepage = 0)
		{
			return std::string(source);
		}
	};

	namespace Details
	{
		template<class TChar, class T>
		inline std::basic_string<TChar> ToString(T&& obj) requires
			requires { { std::declval<T>()->ToString() } -> std::convertible_to<std::basic_string_view<TChar>>; }
		{
			return std::basic_string<TChar>(obj->ToString());
		}

		template<class TChar, class T>
		inline std::basic_string<TChar> ToString(T&& value) requires
			requires { { std::declval<T>().ToString() } -> std::convertible_to<std::basic_string_view<TChar>>; }
		{
			return std::basic_string<TChar>(value.ToString());
		}
	}

	template<class TChar, class... Tx, class TValue>
	std::basic_ostream<TChar, Tx...>& operator <<(std::basic_ostream<TChar, Tx...>& os, TValue&& value) requires
		requires { Details::ToString<TChar>(std::declval<TValue>()); }
	{
		auto ws = Details::ToString<TChar>(std::forward<TValue>(value));
		return os << ws;
	}
}

inline ::libty::String operator "" _s(const char* mstr, size_t len)
{
	return ::libty::String(std::string_view(mstr, len));
}

inline ::libty::String operator "" _s(const wchar_t* wstr, size_t len)
{
	return ::libty::String(std::wstring_view(wstr, len));
}

template<class TValue> requires
	requires { libty::Core::Details::ToString<wchar_t>(std::declval<TValue>()); }
struct std::formatter<TValue, wchar_t> : public std::formatter<std::wstring_view, wchar_t>
{
	template<class UValue, class TFormatContext>
	auto format(UValue&& value, TFormatContext& ctx)
	{
		auto str = libty::Core::Details::ToString<wchar_t>(std::forward<UValue>(value));
		return std::formatter<std::wstring_view, wchar_t>::format(std::move(str), ctx);
	}
};

template<class TValue> requires
	requires { libty::Core::Details::ToString<char>(std::declval<TValue>()); }
struct std::formatter<TValue, wchar_t> : public std::formatter<std::string_view, char>
{
	template<class UValue, class TFormatContext>
	auto format(UValue&& value, TFormatContext& ctx)
	{
		auto str = libty::Core::Details::ToString<char>(std::forward<UValue>(value));
		return std::formatter<std::string_view, char>::format(std::move(str), ctx);
	}
};