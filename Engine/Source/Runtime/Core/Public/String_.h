// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CharType.h"
#include "IntegralTypes.h"
#include "StringComparison.h"
#include "StringSplitOptions.h"
#include "Math_.h"
#include "Linq/Contains.h"
#include <variant>
#include <memory>
#include <concepts>
#include <compare>
#include <filesystem>
#include <ranges>
#include <string_view>
#include <format>
#include <cstring>

namespace Ayla
{
	class CORE_API String
	{
		using string_t = std::basic_string<char_t>;
		using string_view_t = std::basic_string_view<char_t>;

		std::variant<string_view_t, std::shared_ptr<char_t[]>> m_Buf;
		size_t m_Len;
		bool m_bNullTerminate;

	private:
		// FromLiteral specialized.
		inline consteval String(string_view_t str, size_t len, std::in_place_t) noexcept
			: m_Buf(str)
			, m_Len(len)
			, m_bNullTerminate(true)
		{
		}

		inline constexpr String(decltype(m_Buf) buf, size_t len)
			: m_Buf(std::move(buf))
			, m_Len(len)
			, m_bNullTerminate(true)
		{
		}

		inline constexpr const char_t* GetRaw() const noexcept
		{
			const char_t* ptr = nullptr;
			switch (m_Buf.index())
			{
			case 0:
				ptr = std::get<0>(m_Buf).data();
				break;
			case 1:
				ptr = std::get<1>(m_Buf).get();
				break;
			}

			return ptr ? ptr : &Char::NullChar;
		}

		static inline constexpr char_t SafeGet(const char_t* inBuf, size_t inLen, size_t idx, bool bLowerCase = false) noexcept
		{
			if (idx >= inLen || inBuf == nullptr)
			{
				return 0;
			}
			else
			{
				return bLowerCase ? Char::ToLower(inBuf[idx]) : inBuf[idx];
			}
		}

		inline constexpr std::strong_ordering CompareTo(const char_t* inBuf, size_t inLen, StringComparison comparison) const noexcept
		{
			size_t length = Math::Min(m_Len, inLen) + 1;
			const char_t* myBuf = GetRaw();

			bool bLowerCase = comparison == StringComparison::CurrentCultureIgnoreCase;

			for (size_t i = 0; i < length; ++i)
			{
				auto cmp = SafeGet(myBuf, m_Len, i, bLowerCase) <=> SafeGet(inBuf, inLen, i, bLowerCase);
				if (cmp != 0)
				{
					return cmp;
				}
			}

			return std::strong_ordering::equal;
		}

		String& AllocateAssign(const char_t* buf, size_t len);
		String& AllocateAssign(const char16_t* buf, size_t len);

		enum class TrimType
		{
			Head = 0x1,
			Tail = 0x2,
		};

		template<bool View, std::ranges::input_range T> requires std::convertible_to<std::ranges::range_value_t<T>, char_t>
		constexpr String TrimAll(TrimType trimType, const T& chars) const
		{
			if (m_Len == 0)
			{
				return String();
			}

			const char_t* const myBuf = GetRaw();

			size_t head = 0;
			if (((int32)trimType & (int32)TrimType::Head) > 0)
			{
				for (; head < m_Len; ++head)
				{
					char_t wc = SafeGet(myBuf, m_Len, head);
					if ((chars | Linq::Contains(wc)) == false)
					{
						break;
					}
				}
			}

			if (head >= m_Len)
			{
				return String();
			}

			size_t tail = m_Len;
			if (((int32)trimType & (int32)TrimType::Tail) > 0)
			{
				for (; tail > head; --tail)
				{
					char_t wc = SafeGet(myBuf, m_Len, tail - 1);
					if ((chars | Linq::Contains(wc)) == false)
					{
						break;
					}
				}
			}

			if (tail <= head)
			{
				return String();
			}

			size_t span = tail - head;
			if constexpr (View)
			{
				return SubstringView(head, span);
			}
			else
			{
				return Substring(head, span);
			}
		}

		template<std::ranges::input_range T> requires std::convertible_to<std::ranges::range_value_t<T>, string_view_t>
		static String InternalConcat(const T& inElements)
		{
			size_t length = 0;
			for (const string_view_t& elem : inElements)
			{
				length += elem.length();
			}

			decltype(m_Buf) nbuf;
			auto& ptr = nbuf.template emplace<1>(std::make_shared<char_t[]>(length + 1));
			size_t appendIdx = 0;
			for (const string_view_t& elem : inElements)
			{
				memcpy(ptr.get() + appendIdx, elem.data(), sizeof(char_t) * elem.length());
				appendIdx += elem.length();
			}

			ptr[length] = 0;
			return String(std::move(nbuf), length);
		}

	public:
		inline constexpr String() noexcept
			: m_Buf(&Char::NullChar)
			, m_Len(0)
			, m_bNullTerminate(true)
		{
		}

		inline constexpr String(const String& rhs) noexcept
			: m_Buf(rhs.m_Buf)
			, m_Len(rhs.m_Len)
			, m_bNullTerminate(rhs.m_bNullTerminate)
		{
		}

		inline constexpr String(String&& rhs) noexcept
			: m_Buf(std::move(rhs.m_Buf))
			, m_Len(rhs.m_Len)
			, m_bNullTerminate(rhs.m_bNullTerminate)
		{
			rhs.m_Buf = &Char::NullChar;
			rhs.m_Len = 0;
			rhs.m_bNullTerminate = true;
		}

		template<class T>
		explicit inline constexpr String(const T& ch) noexcept requires
			std::same_as<T, char> ||
			std::same_as<T, char_t>
			: String(&ch, 1)
		{
		}

		explicit String(std::string_view str);
		String(const char* buf, size_t len);

		explicit inline String(std::wstring_view sv)
		{
			this->AllocateAssign(sv.data(), sv.length());
		}

		inline String(const char_t* inBuf, size_t inLen)
			: String(string_view_t(inBuf, inLen))
		{
		}

		inline String(const char16_t* buf, size_t len)
		{
			this->AllocateAssign(buf, len);
		}

		inline String(char_t ch, size_t inLen)
		{
			auto& ptr = m_Buf.template emplace<1>(std::make_shared<char_t[]>(inLen + 1));
			auto* rptr = ptr.get();

			for (size_t i = 0; i < inLen; ++i)
			{
				rptr[i] = ch;
			}

			rptr[inLen] = 0;
			m_Len = inLen;
			m_bNullTerminate = true;
		}

	public:
		inline constexpr String& operator =(const String& rhs) noexcept
		{
			m_Buf = rhs.m_Buf;
			m_Len = rhs.m_Len;
			m_bNullTerminate = rhs.m_bNullTerminate;
			return *this;
		}

		inline constexpr String& operator =(String&& rhs) noexcept
		{
			m_Buf = std::move(rhs.m_Buf);
			m_Len = rhs.m_Len;
			m_bNullTerminate = rhs.m_bNullTerminate;
			rhs.m_Buf = &Char::NullChar;
			rhs.m_Len = 0;
			rhs.m_bNullTerminate = true;
			return *this;
		}

		inline constexpr const char_t& operator [](size_t idx) const noexcept
		{
			const char_t* myBuf = this->GetRaw();
			if (myBuf != nullptr)
			{
				return myBuf[idx];
			}
			return Char::NullChar;
		}

		inline constexpr bool operator ==(const String& rhs) const noexcept
		{
			if (m_Len != rhs.m_Len)
			{
				return false;
			}

			return this->operator <=>(rhs) == 0;
		}

		inline constexpr std::strong_ordering operator <=>(const String& rhs) const noexcept
		{
			return this->CompareTo(rhs.GetRaw(), rhs.m_Len, StringComparison::CurrentCulture);
		}

		inline String operator +(const String& rhs) const
		{
			return Concat(*this, rhs);
		}

		inline String& operator +=(const String& rhs)
		{
			return this->operator =(*this + rhs);
		}

		inline explicit operator std::string() const { return AsCodepage(); }
		inline std::string string() const { return (std::string)*this; }

		inline explicit operator std::wstring() const { return std::wstring(this->GetRaw(), m_Len); }
		inline std::wstring wstring() const { return (std::wstring)*this; }

		inline constexpr operator std::wstring_view() const noexcept { return std::wstring_view(this->GetRaw(), m_Len); }
		inline std::wstring_view wstring_view() const { return (std::wstring_view)*this; }

		inline explicit operator std::filesystem::path() const { return std::filesystem::path(this->GetRaw(), this->GetRaw() + m_Len); }
		inline std::filesystem::path path() const { return (std::filesystem::path)*this; }

		template<class... TArgs>
		inline consteval operator std::basic_format_string<char_t, TArgs...>() const noexcept
		{
			return std::get<0>(m_Buf);
		}

		inline constexpr explicit operator const char_t* () const noexcept
		{
			return this->GetRaw();
		}

		inline constexpr explicit operator size_t () const noexcept
		{
			return m_Len;
		}

		inline constexpr explicit operator bool() const noexcept
		{
			return m_Len != 0;
		}

	public:
#pragma region IEnumerable<char_t>
		inline constexpr const char_t* begin() const noexcept
		{
			if (m_Len == 0)
			{
				return &Char::NullChar;
			}
			else
			{
				return GetRaw();
			}
		}

		inline constexpr const char_t* end() const noexcept
		{
			if (m_Len == 0)
			{
				return &Char::NullChar;
			}
			else
			{
				return GetRaw() + m_Len;
			}
		}

		[[nodiscard]] inline constexpr size_t size() const noexcept
		{
			return m_Len + 1;
		}
#pragma endregion

#pragma region IEquatable<String>
	public:
		[[nodiscard]] constexpr inline bool Equals(const String& rhs) const noexcept
		{
			return Equals(rhs, StringComparison::CurrentCulture);
		}
#pragma endregion
#pragma region IComparable<String>
	public:
		[[nodiscard]] constexpr std::strong_ordering CompareTo(const String& rhs) const noexcept
		{
			return this->CompareTo(rhs.GetRaw(), rhs.m_Len, StringComparison::CurrentCulture);
		}
#pragma endregion

	public:
		[[nodiscard]] inline constexpr bool Equals(const String& rhs, StringComparison comparison) const noexcept
		{
			if (m_Len != rhs.m_Len)
			{
				return false;
			}

			return this->CompareTo(rhs.GetRaw(), rhs.m_Len, comparison) == 0;
		}

		[[nodiscard]] inline constexpr String ToString() const noexcept
		{
			return *this;
		}

		[[nodiscard]] inline String Clone() const
		{
			return String(this->GetRaw(), this->m_Len);
		}

		[[nodiscard]] inline constexpr size_t length() const noexcept
		{
			return (size_t)*this;
		}

		[[nodiscard]] inline constexpr const char_t* c_str() const noexcept
		{
			return (const char_t*)*this;
		}

		[[nodiscard]] inline const char_t* lock_thread_local() const
		{
			static thread_local string_t scoped;
			scoped = wstring_view();
			return scoped.c_str();
		}

		[[nodiscard]] constexpr std::strong_ordering CompareTo(const String& rhs, StringComparison comparison) const noexcept
		{
			return this->CompareTo(rhs.GetRaw(), rhs.m_Len, comparison);
		}

		[[nodiscard]] inline constexpr bool Contains(char_t ch, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			return IndexOf(ch, 0, (size_t)-1, comparison) != -1;
		}

		[[nodiscard]] inline constexpr bool Contains(const String& compare, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			return IndexOf(compare, 0, (size_t)-1, comparison) != -1;
		}

		[[nodiscard]] inline constexpr bool Contains(const String& compare, size_t indexOf, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			return IndexOf(compare, indexOf, (size_t)-1, comparison) != -1;
		}

		[[nodiscard]] inline constexpr bool Contains(const String& compare, size_t indexOf, size_t length, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			return IndexOf(compare, indexOf, length, comparison) != -1;
		}

		[[nodiscard]] inline constexpr size_t IndexOf(char_t ch, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			return IndexOf(ch, 0, (size_t)-1, comparison);
		}

		[[nodiscard]] inline constexpr size_t IndexOf(char_t ch, size_t indexOf, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			return IndexOf(ch, indexOf, (size_t)-1, comparison);
		}

		[[nodiscard]] constexpr size_t IndexOf(char_t ch, size_t indexOf, size_t length, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			if (length > m_Len || length == -1)
			{
				length = m_Len;
			}

			if (length == 0)
			{
				return (size_t)-1;
			}

			if (m_Len == 0)
			{
				return (size_t)-1;
			}

			const char_t* const buf1 = this->GetRaw();
			const bool bLowerCase = comparison == StringComparison::CurrentCultureIgnoreCase;

			if (bLowerCase)
			{
				ch = (char_t)std::tolower(ch);
			}

			for (size_t i = indexOf; i < length; ++i)
			{
				char_t lch = SafeGet(buf1, length, i, bLowerCase);
				char_t rch = ch;

				if (lch == rch)
				{
					return i;
				}
			}

			return (size_t)-1;
		}

		[[nodiscard]] inline constexpr size_t IndexOf(const String& compare, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			return IndexOf(compare, 0, (size_t)-1, comparison);
		}

		[[nodiscard]] inline constexpr size_t IndexOf(const String& compare, size_t indexOf, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			return IndexOf(compare, indexOf, (size_t)-1, comparison);
		}

		[[nodiscard]] constexpr size_t IndexOf(const String& compare, size_t indexOf, size_t length, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			if (indexOf > m_Len)
			{
				return (size_t)-1;
			}

			size_t maxLength = m_Len - indexOf;
			if (length > maxLength || length == -1)
			{
				length = maxLength;
			}

			if (length == 0)
			{
				return (size_t)-1;
			}

			if (!compare)
			{
				return (size_t)-1;
			}

			if (compare.m_Len > length)
			{
				return (size_t)-1;
			}

			if (indexOf >= m_Len - compare.m_Len)
			{
				return (size_t)-1;
			}

			const char_t* const buf1 = this->GetRaw();
			const char_t* const buf2 = compare.GetRaw();
			const bool bLowerCase = comparison == StringComparison::CurrentCultureIgnoreCase;
			const size_t lastIndex = m_Len - compare.m_Len;

			size_t compares = 0;
			size_t i;
			for (i = indexOf; i < m_Len;)
			{
				char_t lch = SafeGet(buf1, m_Len, i, bLowerCase);
				char_t rch = SafeGet(buf2, compare.m_Len, compares, bLowerCase);

				if (lch == rch)
				{
					++compares;
					if (compares == compare.m_Len)
					{
						++i;
						break;
					}
				}
				else
				{
					if (i > lastIndex)
					{
						break;
					}

					if (compares > 0)
					{
						compares = 0;
						continue;
					}
				}

				++i;
			}

			if (compares == compare.m_Len)
			{
				return i - compare.m_Len;
			}
			else
			{
				return (size_t)-1;
			}
		}

		[[nodiscard]] inline constexpr size_t LastIndexOf(char_t ch, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			return LastIndexOf(ch, 0, (size_t)-1, comparison);
		}

		[[nodiscard]] inline constexpr size_t LastIndexOf(char_t ch, size_t indexOf, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			return LastIndexOf(ch, indexOf, (size_t)-1, comparison);
		}

		[[nodiscard]] constexpr size_t LastIndexOf(char_t ch, size_t indexOf, size_t length, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			if (length > m_Len || length == -1)
			{
				length = m_Len;
			}

			if (length == 0)
			{
				return (size_t)-1;
			}

			if (m_Len == 0)
			{
				return (size_t)-1;
			}

			const char_t* const buf1 = this->GetRaw();
			const bool bLowerCase = comparison == StringComparison::CurrentCultureIgnoreCase;

			if (bLowerCase)
			{
				ch = (char_t)std::tolower(ch);
			}

			for (size_t i = indexOf; i < length; ++i)
			{
				size_t index = length - i - 1;
				char_t lch = SafeGet(buf1, length, index, bLowerCase);
				char_t rch = ch;

				if (lch == rch)
				{
					return index;
				}
			}

			return (size_t)-1;
		}

		[[nodiscard]] constexpr bool StartsWith(const String& compare, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			if (m_Len == 0)
			{
				return false;
			}

			if (!compare)
			{
				return false;
			}

			if (compare.m_Len > m_Len)
			{
				return false;
			}

			const char_t* const buf1 = this->GetRaw();
			const char_t* const buf2 = compare.GetRaw();
			const bool bLowerCase = comparison == StringComparison::CurrentCultureIgnoreCase;

			size_t compares = 0;
			for (size_t i = 0; i < compare.m_Len; ++i)
			{
				char_t lch = SafeGet(buf1, m_Len, i, bLowerCase);
				char_t rch = SafeGet(buf2, compare.m_Len, compares, bLowerCase);

				if (lch == rch)
				{
					++compares;
					if (compares == compare.m_Len)
					{
						return true;
					}
				}
				else
				{
					return false;
				}
			}

			return false;
		}

		[[nodiscard]] constexpr bool EndsWith(const String& compare, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			if (m_Len == 0)
			{
				return false;
			}

			if (!compare)
			{
				return false;
			}

			if (compare.m_Len > m_Len)
			{
				return false;
			}

			const char_t* const buf1 = this->GetRaw();
			const char_t* const buf2 = compare.GetRaw();
			const bool bLowerCase = comparison == StringComparison::CurrentCultureIgnoreCase;
			const size_t endIndex = m_Len - compare.m_Len;

			size_t compares = 0;
			for (size_t i = m_Len - 1; i >= endIndex && i != -1; --i)
			{
				char_t lch = SafeGet(buf1, m_Len, i, bLowerCase);
				char_t rch = SafeGet(buf2, compare.m_Len, compare.m_Len - (compares + 1), bLowerCase);

				if (lch == rch)
				{
					++compares;
					if (compares == compare.m_Len)
					{
						return true;
					}
				}
				else
				{
					return false;
				}
			}

			return false;
		}

		[[nodiscard]] inline constexpr bool IsStringView() const noexcept
		{
			return m_Buf.index() == 0;
		}

		[[nodiscard]] inline constexpr bool IsNullTerminate() const noexcept
		{
			return m_bNullTerminate;
		}

		template<std::ranges::input_range TCharArray>
			requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
		[[nodiscard]] inline constexpr size_t IndexOfAny(const TCharArray& chars, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			return IndexOfAny(chars, 0, comparison);
		}

		template<std::ranges::input_range TCharArray>
			requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
		[[nodiscard]] constexpr size_t IndexOfAny(const TCharArray& chars, size_t indexOf, StringComparison comparison = StringComparison::CurrentCulture) const noexcept
		{
			if (m_Len == 0)
			{
				return (size_t)-1;
			}

			if (std::ranges::size(chars) == 0)
			{
				return (size_t)-1;
			}

			const char_t* const myBuf = this->GetRaw();
			const bool bLowerCase = comparison == StringComparison::CurrentCultureIgnoreCase;

			for (size_t i = indexOf; i < m_Len; ++i)
			{
				char_t ch = SafeGet(myBuf, m_Len, i, bLowerCase);

				bool bFound = false;
				if (bLowerCase)
				{
					bFound = chars | Linq::Contains([&ch](char_t wc)
						{
							return ch == (char_t)std::tolower(wc);
						});
				}
				else
				{
					bFound = chars | Linq::Contains(ch);
				}

				if (bFound)
				{
					return i;
				}
			}

			return (size_t)-1;
		}

		[[nodiscard]] inline constexpr bool IsEmpty() const noexcept
		{
			return m_Len == 0;
		}

		[[nodiscard]] inline constexpr bool IsWhiteSpace() const noexcept
		{
			auto trim = TrimStartView();
			return trim.IsEmpty();
		}

		[[nodiscard]] inline constexpr bool IsValidIndex(size_t index) const noexcept
		{
			return index < m_Len;
		}

		template<bool View = false>
		[[nodiscard]] inline constexpr String Trim() const noexcept(View)
		{
			return Trim<View>(Char::WhiteSpaceChars);
		}

		[[nodiscard]] inline constexpr String TrimView() const
		{
			return TrimView(Char::WhiteSpaceChars);
		}

		template<bool View = false, std::ranges::input_range TCharArray>
			requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
		[[nodiscard]] inline constexpr String Trim(const TCharArray& chars) const noexcept(View)
		{
			return this->TrimAll<View>((TrimType)((int32)TrimType::Head | (int32)TrimType::Tail), chars);
		}

		template<std::ranges::input_range TCharArray>
			requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
		[[nodiscard]] inline constexpr String TrimView(const TCharArray& chars) const
		{
			return this->TrimAll<true>((TrimType)((int32)TrimType::Head | (int32)TrimType::Tail), chars);
		}

		template<bool View = false, std::convertible_to<char_t>... TCharSequence>
		[[nodiscard]] inline constexpr String Trim(const TCharSequence&... chars) const noexcept(View)
		{
			return this->Trim<View>(std::array{ (char_t)chars... });
		}

		template<std::convertible_to<char_t>... TCharSequence>
		[[nodiscard]] inline constexpr String TrimView(const TCharSequence&... chars) const
		{
			return this->TrimView(std::array{ (char_t)chars... });
		}

		template<bool View = false>
		[[nodiscard]] inline constexpr String TrimStart() const noexcept(View)
		{
			return TrimStart<View>(Char::WhiteSpaceChars);
		}

		[[nodiscard]] inline constexpr String TrimStartView() const
		{
			return TrimStartView(Char::WhiteSpaceChars);
		}

		template<bool View = false, std::ranges::input_range TCharArray>
			requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
		[[nodiscard]] inline constexpr String TrimStart(const TCharArray& chars) const noexcept(View)
		{
			return this->TrimAll<View>(TrimType::Head, chars);
		}

		template<std::ranges::input_range TCharArray>
			requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
		[[nodiscard]] inline constexpr String TrimStartView(const TCharArray& chars) const
		{
			return this->TrimAll<true>(TrimType::Head, chars);
		}

		template<bool View = false, std::convertible_to<char_t>... TCharSequence>
		[[nodiscard]] inline constexpr String TrimStart(const TCharSequence&... chars) const noexcept(View)
		{
			return TrimStart<View>(std::array{ (char_t)chars... });
		}

		template<std::convertible_to<char_t>... TCharSequence>
		[[nodiscard]] inline constexpr String TrimStartView(const TCharSequence&... chars) const
		{
			return TrimStartView(std::array{ (char_t)chars... });
		}

		template<bool View = false>
		[[nodiscard]] inline constexpr String TrimEnd() const noexcept(View)
		{
			return TrimEnd<View>(Char::WhiteSpaceChars);
		}

		[[nodiscard]] inline constexpr String TrimEndView() const
		{
			return TrimEndView(Char::WhiteSpaceChars);
		}

		template<bool View = false, std::ranges::input_range TCharArray>
			requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
		[[nodiscard]] inline constexpr String TrimEnd(const TCharArray& chars) const noexcept(View)
		{
			return this->TrimAll<View>(TrimType::Tail, chars);
		}

		template<std::ranges::input_range TCharArray>
			requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
		[[nodiscard]] inline constexpr String TrimEndView(const TCharArray& chars) const
		{
			return this->TrimAll<true>(TrimType::Tail, chars);
		}

		template<bool View = false, std::convertible_to<char_t>... TCharSequence>
		[[nodiscard]] inline constexpr String TrimEnd(const TCharSequence&... chars) const noexcept(View)
		{
			return TrimEnd<View>(std::array{ (char_t)chars... });
		}

		template<std::convertible_to<char_t>... TCharSequence>
		[[nodiscard]] inline constexpr String TrimEndView(const TCharSequence&... chars) const
		{
			return TrimEndView(std::array{ (char_t)chars... });
		}

		template<bool View = false>
		[[nodiscard]] constexpr String Substring(size_t startIndex, size_t length = -1) const noexcept(View)
		{
			if (startIndex >= m_Len)
			{
				return GetEmpty();
			}

			if (length > m_Len - startIndex)
			{
				length = m_Len - startIndex;
			}

			length = std::min(length, m_Len - startIndex);
			if constexpr (View)
			{
				return String::FromLiteral(std::wstring_view(this->GetRaw() + startIndex, length));
			}
			else
			{
				return String(std::wstring_view(this->GetRaw() + startIndex, length));
			}
		}

		[[nodiscard]] constexpr String SubstringView(size_t startIndex, size_t length = -1) const noexcept
		{
			return Substring<true>(startIndex, length);
		}

		template<bool View = false>
		[[nodiscard]] inline constexpr std::vector<String> Split(char_t separator, StringSplitOptions options = StringSplitOptions::None) const noexcept(View)
		{
			std::array<char_t, 1> seps{ separator };
			return Split<View>(seps, options);
		}

		[[nodiscard]] inline constexpr std::vector<String> SplitView(char_t separator, StringSplitOptions options = StringSplitOptions::None) const noexcept
		{
			std::array<char_t, 1> seps{ separator };
			return SplitView(seps, options);
		}

		template<bool View = false, std::ranges::input_range TCharArray>
			requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
		[[nodiscard]] constexpr std::vector<String> Split(const TCharArray& separators, StringSplitOptions options = StringSplitOptions::None) const noexcept(View)
		{
			std::vector<String> results;

			const bool bRemoveEmpty = (int32)(options & StringSplitOptions::RemoveEmptyEntries) > 0;
			const bool bTrim = (int32)(options & StringSplitOptions::TrimEntries) > 0;

			for (size_t i = 0; i < m_Len;)
			{
				std::optional<String> view;
				size_t seekp = IndexOfAny(separators, i);

				if (seekp == -1)
				{
					view = Substring<View>(i);
					i = (size_t)-1;
				}
				else
				{
					size_t length = seekp - i;
					if (length != 0 || !bRemoveEmpty)
					{
						view = Substring<View>(i, length);
					}
					i = seekp + 1;
				}

				if (view)
				{
					if (bTrim)
					{
						view = view->Trim<View>();
						if (bRemoveEmpty && view->m_Len == 0)
						{
							continue;
						}
					}

					results.emplace_back(view.value());
				}
			}

			return results;
		}

		template<std::ranges::input_range TCharArray>
			requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
		[[nodiscard]] constexpr std::vector<String> SplitView(const TCharArray& separators, StringSplitOptions options = StringSplitOptions::None) const noexcept
		{
			return Split<true>(separators, options);
		}

		template<bool View = false>
		[[nodiscard]] constexpr std::vector<String> Split(const String& separator, StringSplitOptions options = StringSplitOptions::None) const noexcept(View)
		{
			std::vector<String> results;

			const bool bRemoveEmpty = (int32)(options & StringSplitOptions::RemoveEmptyEntries) > 0;
			const bool bTrim = (int32)(options & StringSplitOptions::TrimEntries) > 0;

			for (size_t i = 0; i < m_Len;)
			{
				std::optional<String> view;
				size_t seekp = IndexOf(separator, i);

				if (seekp == -1)
				{
					view = Substring<View>(i);
					i = (size_t)-1;
				}
				else
				{
					size_t length = seekp - i;
					if (length != 0 || !bRemoveEmpty)
					{
						view = Substring<View>(i, length);
					}
					i = seekp + separator.m_Len;
				}

				if (view)
				{
					if (bTrim)
					{
						view = view->Trim<View>();
						if (bRemoveEmpty && view->m_Len == 0)
						{
							continue;
						}
					}

					results.emplace_back(view.value());
				}
			}

			return results;
		}

		[[nodiscard]] constexpr std::vector<String> SplitView(const String& separator, StringSplitOptions options = StringSplitOptions::None) const noexcept
		{
			return Split<true>(separator, options);
		}

		template<class TOp>
		[[nodiscard]] String Transform(TOp&& op) const requires
			std::convertible_to<std::invoke_result_t<TOp, char_t>, char_t>
		{
			decltype(m_Buf) nbuf;
			auto& ptr = nbuf.template emplace<1>(std::make_shared<char_t[]>(m_Len + 1));
			const char_t* const myBuf = this->GetRaw();

			for (size_t i = 0; i < m_Len; ++i)
			{
				ptr[i] = (char_t)op(myBuf[i]);
			}

			String s;
			s.m_Buf = std::move(nbuf);
			s.m_Len = m_Len;
			return s;
		}

		[[nodiscard]] inline String ToLower() const
		{
			return Transform((int(*)(int))std::tolower);
		}

		[[nodiscard]] inline String ToUpper() const
		{
			return Transform((int(*)(int))std::toupper);
		}

		[[nodiscard]] String Replace(const String& oldValue, const String& newValue, StringComparison comparison = StringComparison::CurrentCulture) const
		{
			const char_t* const myBuf = this->GetRaw();

			std::vector<std::wstring_view> concats;

			for (size_t i = 0;;)
			{
				size_t indexOf = IndexOf(oldValue, i, comparison);
				if (indexOf == -1)
				{
					size_t len = m_Len - i;
					concats.emplace_back(std::wstring_view(myBuf + i, len));
					break;
				}
				else
				{
					size_t len = indexOf - i;
					concats.emplace_back(std::wstring_view(myBuf + i, len));
					concats.emplace_back(std::wstring_view(newValue.GetRaw(), newValue.m_Len));
					i = indexOf + oldValue.m_Len;
				}
			}

			return InternalConcat(concats);
		}

		[[nodiscard]] String ReplaceAt(size_t indexOf, size_t length, String newValue) const
		{
			if (!newValue)
			{
				return *this;
			}

			if (indexOf >= m_Len)
			{
				return *this;
			}

			length = std::min(length, m_Len - indexOf);
			size_t tail = indexOf + length;

			std::array<std::wstring_view, 3> concats;
			concats[0] = std::wstring_view(this->GetRaw(), indexOf);
			concats[1] = std::wstring_view(newValue.GetRaw(), newValue.m_Len);
			concats[2] = std::wstring_view(this->GetRaw() + tail, m_Len - tail);
			return InternalConcat(concats);
		}

		[[nodiscard]] inline String Insert(size_t indexOf, String newValue) const
		{
			return ReplaceAt(indexOf, 0, newValue);
		}

		[[nodiscard]] inline String Insert(size_t indexOf, char_t ch) const
		{
			char_t localBuf[2] = { ch, 0 };
			return Insert(indexOf, String::FromLiteral(localBuf));
		}

		[[nodiscard]] inline String Remove(size_t indexOf, size_t length = -1) const
		{
			const size_t rem = m_Len - indexOf;
			if (length > rem || length == -1)
			{
				length = rem;
			}

			std::array<std::wstring_view, 2> concats;
			concats[0] = std::wstring_view(this->GetRaw(), indexOf);
			size_t app = indexOf + length;
			concats[1] = std::wstring_view(this->GetRaw() + app, m_Len - app);
			return InternalConcat(concats);
		}

		[[nodiscard]] inline String Quotes(char_t ch = '"') const
		{
			std::array<std::wstring_view, 3> concats;
			concats[0] = std::wstring_view(&ch, 1);
			concats[1] = std::wstring_view(this->GetRaw(), m_Len);
			concats[2] = std::wstring_view(&ch, 1);
			return InternalConcat(concats);
		}

	public:
		[[nodiscard]] static inline constexpr String FromLiteral(std::wstring_view str) noexcept
		{
			return String(decltype(m_Buf)(str), str.length());
		}

		[[nodiscard]] static String FromLiteral(std::string_view str);

	public:
		[[nodiscard]] static String FromCodepage(std::string_view str, int32 codepage = 0);
		[[nodiscard]] std::string AsCodepage(int32 codepage = 0) const;

	public:
		template<class... TArgs>
		[[nodiscard]] static String Format(String formatStr, TArgs... args)
		{
			return String(std::vformat(formatStr.wstring_view(), std::make_wformat_args(args...)));
		}

		template<std::ranges::input_range TStringArray>
			requires std::convertible_to<std::ranges::range_value_t<TStringArray>, String>
		[[nodiscard]] static String Concat(const TStringArray& strings)
		{
			std::vector<std::wstring_view> buffers;

			if constexpr (std::ranges::sized_range<TStringArray>)
			{
				buffers.reserve(std::ranges::size(strings));
			}

			for (const String& item : strings)
			{
				buffers.emplace_back(std::wstring_view(item.GetRaw(), item.m_Len));
			}

			return InternalConcat(buffers);
		}

		template<std::convertible_to<String>... TStringSequence>
		[[nodiscard]] static inline String Concat(const TStringSequence&... strings)
		{
			return Concat(std::array{ (String)strings... });
		}

		template<std::ranges::input_range TStringArray>
			requires std::convertible_to<std::ranges::range_value_t<TStringArray>, String>
		[[nodiscard]] static String Join(const String& separator, TStringArray&& strings)
		{
			std::vector<String> concats;

			if constexpr (std::ranges::sized_range<TStringArray>)
			{
				concats.reserve(std::ranges::size(strings));
			}

			for (const auto& item : strings)
			{
				if (concats.size() > 0)
				{
					concats.emplace_back(separator);
				}

				concats.emplace_back(item);
			}

			return Concat(concats);
		}

		template<std::convertible_to<String>... TStringSequence>
		[[nodiscard]] static inline String Join(const String& separator, const TStringSequence&... strings)
		{
			return Join(separator, std::array{ (String)strings... });
		}

		[[nodiscard]] static inline constexpr String GetEmpty() noexcept
		{
			// no allocate
			return String();
		}
	};

	// Declared in CharType.h
	constexpr FORCEINLINE String Char::ToStringView(const char_t& ch) noexcept
	{
		return String::FromLiteral(std::wstring_view(&ch, 1));
	}

	FORCEINLINE String Char::ToString(char_t ch)
	{
		return ToStringView(ch).Clone();
	}

#define TEXT(X) (::Ayla::String::FromLiteral(L ## X))
}

template<>
struct std::formatter<Ayla::String, wchar_t> : public std::formatter<std::wstring_view, wchar_t>
{
	template<class TFormatContext>
	auto format(const Ayla::String& str, TFormatContext& context) const
	{
		return std::formatter<std::wstring_view, wchar_t>::format((std::wstring_view)str, context);
	}
};

template<>
struct std::formatter<Ayla::String, char> : public std::formatter<std::string, char>
{
	template<class TFormatContext>
	auto format(const Ayla::String& str, TFormatContext& context) const
	{
		return std::formatter<std::string_view, char>::format((std::string)str, context);
	}
};

template<class T, class TChar> requires requires { { std::declval<T>().ToString() } -> std::convertible_to<Ayla::String>; }
struct std::formatter<T, TChar> : public std::formatter<::Ayla::String, TChar>
{
	template<class U, class TFormatContext>
	auto format(U&& obj, TFormatContext& context) const
	{
		return std::formatter<Ayla::String, TChar>::format(std::forward<U>(obj).ToString(), context);
	}
};