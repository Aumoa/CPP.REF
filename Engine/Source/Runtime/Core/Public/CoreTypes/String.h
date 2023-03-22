// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CharType.h"
#include "Misc/StringComparison.h"
#include "Misc/StringSplitOptions.h"
#include "Linq/Array.h"
#include "Misc/Misc.h"
#include "Misc/CompatibleVariadic.h"
#include "Misc/PlatformMisc.h"
#include <variant>
#include <array>
#include <memory>
#include <string_view>
#include <utility>
#include <compare>
#include <cstring>
#include <format>
#include <vector>
#include <unordered_map>

namespace lib::details
{
	template<class T>
	concept _to_string = requires
	{
		{ std::declval<T>().ToString() };
	};
}

class CORE_API String
{
	std::variant<const char_t*, std::shared_ptr<char_t[]>> _buf;
	size_t _len;

private:
	static inline constexpr const char_t _null_char = 0;
	static inline constexpr const std::array _whitespace_chars = { L' ', L'\t', L'\r', L'\n', L'\b' };

private:
	inline constexpr String(decltype(_buf) buf, size_t len)
		: _buf(std::move(buf))
		, _len(len)
	{
	}

	inline constexpr const char_t* _get_raw() const noexcept
	{
		const char_t* buf = nullptr;
		switch (_buf.index())
		{
		case 0:
			buf = std::get<0>(_buf);
			break;
		case 1:
			buf = std::get<1>(_buf).get();
			break;
		}

		return buf ? buf : &_null_char;
	}

	static inline constexpr char_t _safe_get(const char_t* buf, size_t len, size_t idx, bool lowerCase = false) noexcept
	{
		if (idx >= len || buf == nullptr)
		{
			return 0;
		}
		else
		{
			return lowerCase ? std::tolower(buf[idx]) : buf[idx];
		}
	}

	inline constexpr std::strong_ordering _compare_to(const char_t* buf, size_t len, EStringComparison comparison) const noexcept
	{
		size_t length = std::min(_len, len) + 1;
		const char_t* mybuf = this->_get_raw();

		bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;

		for (size_t i = 0; i < length; ++i)
		{
			auto cmp = _safe_get(mybuf, _len, i, lowerCase) <=> _safe_get(buf, len, i, lowerCase);
			if (cmp != 0)
			{
				return cmp;
			}
		}

		return std::strong_ordering::equal;
	}

	inline static constexpr bool _compare_string_view(std::wstring_view lhs, std::wstring_view rhs, bool lowerCase)
	{
		if (!lowerCase)
		{
			return lhs == rhs;
		}
		else
		{
			if (lhs.length() != rhs.length())
			{
				return false;
			}

			for (size_t i = 0; i < lhs.length(); ++i)
			{
				if (std::tolower(lhs[i]) != std::tolower(rhs[i]))
				{
					return false;
				}
			}

			return true;
		}
	}

	String& _allocate_assign(const char_t* buf, size_t len)
	{
		auto& ptr = _buf.emplace<1>(std::make_shared<wchar_t[]>(len + 1));
		memcpy(ptr.get(), buf, sizeof(wchar_t) * len);
		ptr[len] = 0;
		_len = len;
		return *this;
		throw;
	}

	enum class ETrimType
	{
		Head = 0x1,
		Tail = 0x2,
	};

	template<IArray<char_t> TArray>
	constexpr String _trim_all(ETrimType tt, const TArray& chars) const
	{
		if (_len == 0)
		{
			return String();
		}

		const char_t* const buf = _get_raw();

		size_t head = 0;
		if (((int32)tt & (int32)ETrimType::Head) > 0)
		{
			for (; head < _len; ++head)
			{
				char_t wc = _safe_get(buf, _len, head);
				if (!Linq::Contains(&chars, wc))
				{
					break;
				}
			}
		}

		if (head >= _len)
		{
			return String();
		}

		size_t tail = _len;
		if (((int32)tt & (int32)ETrimType::Tail) > 0)
		{
			for (; tail > head; --tail)
			{
				char_t wc = _safe_get(buf, _len, tail - 1);
				if (!Linq::Contains(&chars, wc))
				{
					break;
				}
			}
		}

		if (tail <= head)
		{
			return String();
		}

		size_t len = tail - head;
		return Substring(head, len);
	}
	
	template<IArray<std::wstring_view> TArray>
	static String _concat(const TArray& concat)
	{
		size_t length = 0;
		for (const std::wstring_view& item : concat)
		{
			length += item.length();
		}

		decltype(_buf) nbuf;
		auto& ptr = nbuf.emplace<1>(std::make_shared<char_t[]>(length + 1));
		size_t appendIdx = 0;
		for (const std::wstring_view& item : concat)
		{
			memcpy(ptr.get() + appendIdx, item.data(), sizeof(char_t) * item.length());
			appendIdx += item.length();
		}

		ptr[length] = 0;
		return String(std::move(nbuf), length);
	}

public:
	inline constexpr String() noexcept
		: _len(0)
	{
	}

	inline constexpr String(const String& rhs) noexcept
		: _buf(rhs._buf)
		, _len(rhs._len)
	{
	}

	inline constexpr String(String&& rhs) noexcept
		: _buf(std::move(rhs._buf))
		, _len(rhs._len)
	{
		rhs._len = 0;
	}

	template<class T>
	explicit inline constexpr String(const T& ch) noexcept requires
		std::same_as<T, char> ||
		std::same_as<T, char_t>
		: String(&ch, 1)
	{
	}

	explicit inline String(std::string_view s)
		: String(PlatformMisc::FromCodepage(s))
	{
	}

	inline String(const char* buf, size_t len)
		: String(PlatformMisc::FromCodepage(std::string_view(buf, len)))
	{
	}

	explicit inline String(std::wstring_view s)
	{
		this->_allocate_assign(s.data(), s.length());
	}

	inline String(const char_t* buf, size_t len)
		: String(std::wstring_view(buf, len))
	{
	}

	inline String(char_t ch, size_t len)
	{
		auto& ptr = _buf.emplace<1>(std::make_shared<char_t[]>(len + 1));
		auto* rptr = ptr.get();

		for (size_t i = 0; i < len; ++i)
		{
			rptr[i] = ch;
		}

		rptr[len] = 0;
		_len = len;
	}

public:
	inline constexpr String& operator =(const String& rhs) noexcept
	{
		_buf = rhs._buf;
		_len = rhs._len;
		return *this;
	}

	inline constexpr String& operator =(String&& rhs) noexcept
	{
		_buf = std::move(rhs._buf);
		_len = rhs._len;
		rhs._len = 0;
		return *this;
	}

	inline constexpr const char_t& operator [](size_t idx) const noexcept
	{
		const char_t* buf = this->_get_raw();
		if (buf != nullptr)
		{
			return buf[idx];
		}
		return _null_char;
	}

	inline constexpr bool operator ==(const String& rhs) const noexcept
	{
		if (_len != rhs._len)
		{
			return false;
		}

		return this->operator <=>(rhs) == 0;
	}

	inline constexpr std::strong_ordering operator <=>(const String& rhs) const noexcept
	{
		return this->_compare_to(rhs._get_raw(), rhs._len, EStringComparison::CurrentCulture);
	}

	inline String operator +(const String& rhs) const
	{
		return Concat(*this, rhs);
	}

	inline String& operator +=(const String& rhs)
	{
		return this->operator =(*this + rhs);
	}

	inline explicit operator std::string() const noexcept
	{
		return PlatformMisc::AsCodepage(std::wstring_view(this->_get_raw(), _len));
	}

	inline explicit operator std::wstring() const noexcept
	{
		return std::wstring(this->_get_raw(), _len);
	}

	inline constexpr operator std::wstring_view() const noexcept
	{
		return std::wstring_view(this->_get_raw(), _len);
	}

	inline constexpr explicit operator const char_t* () const noexcept
	{
		return this->_get_raw();
	}

	inline constexpr explicit operator size_t () const noexcept
	{
		return _len;
	}

	inline constexpr explicit operator bool() const noexcept
	{
		return _len != 0;
	}

public:
#pragma region IEnumerable<char_t>
	inline constexpr const char_t* begin() const noexcept
	{
		if (_len == 0)
		{
			return &_null_char;
		}
		else
		{
			return _get_raw();
		}
	}

	inline constexpr const char_t* end() const noexcept
	{
		if (_len == 0)
		{
			return &_null_char;
		}
		else
		{
			return _get_raw() + _len;
		}
	}

	[[nodiscard]] inline constexpr size_t size() const noexcept
	{
		return _len + 1;
	}
#pragma endregion

#pragma region IEquatable<String>
public:
	[[nodiscard]] constexpr inline bool Equals(const String& rhs) const noexcept
	{
		return Equals(rhs, EStringComparison::CurrentCulture);
	}
#pragma endregion
#pragma region IComparable<String>
public:
	[[nodiscard]] constexpr std::strong_ordering CompareTo(const String& rhs) const noexcept
	{
		return this->_compare_to(rhs._get_raw(), rhs._len, EStringComparison::CurrentCulture);
	}
#pragma endregion

public:
	[[nodiscard]] inline constexpr bool Equals(const String& rhs, EStringComparison comparison) const noexcept
	{
		if (_len != rhs._len)
		{
			return false;
		}

		return this->_compare_to(rhs._get_raw(), rhs._len, comparison) == 0;
	}

	[[nodiscard]] inline constexpr String ToString() const noexcept
	{
		return *this;
	}

	[[nodiscard]] inline constexpr size_t length() const noexcept
	{
		return (size_t)*this;
	}

	[[nodiscard]] inline constexpr const char_t* c_str() const noexcept
	{
		return (const char_t*)*this;
	}

	[[nodiscard]] constexpr std::strong_ordering CompareTo(const String& rhs, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return this->_compare_to(rhs._get_raw(), rhs._len, comparison);
	}

	[[nodiscard]] inline constexpr bool Contains(const String& compare, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(compare, 0, -1, comparison) != -1;
	}

	[[nodiscard]] inline constexpr bool Contains(const String& compare, size_t indexOf, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(compare, indexOf, -1, comparison) != -1;
	}

	[[nodiscard]] inline constexpr bool Contains(const String& compare, size_t indexOf, size_t length, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(compare, indexOf, length, comparison) != -1;
	}

	[[nodiscard]] inline constexpr size_t IndexOf(char_t ch, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(ch, 0, -1, comparison);
	}

	[[nodiscard]] inline constexpr size_t IndexOf(char_t ch, size_t indexOf, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(ch, indexOf, -1, comparison);
	}

	[[nodiscard]] constexpr size_t IndexOf(char_t ch, size_t indexOf, size_t length, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		if (length > _len || length == -1)
		{
			length = _len;
		}

		if (length == 0)
		{
			return -1;
		}

		if (_len == 0)
		{
			return -1;
		}

		const char_t* const buf1 = this->_get_raw();
		const bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;

		if (lowerCase)
		{
			ch = std::tolower(ch);
		}

		for (size_t i = indexOf; i < length; ++i)
		{
			char_t lc = _safe_get(buf1, length, i, lowerCase);
			char_t rc = ch;

			if (lc == rc)
			{
				return i;
			}
		}

		return -1;
	}

	[[nodiscard]] inline constexpr size_t IndexOf(const String& compare, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(compare, 0, -1, comparison);
	}

	[[nodiscard]] inline constexpr size_t IndexOf(const String& compare, size_t indexOf, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(compare, indexOf, -1, comparison);
	}

	[[nodiscard]] constexpr size_t IndexOf(const String& compare, size_t indexOf, size_t length, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		if (indexOf > _len)
		{
			return -1;
		}

		size_t max_len = _len - indexOf;
		if (length > max_len || length == -1)
		{
			length = max_len;
		}

		if (length == 0)
		{
			return -1;
		}

		if (!compare)
		{
			return -1;
		}

		if (compare._len > length)
		{
			return -1;
		}

		if (indexOf >= length - compare._len)
		{
			return -1;
		}

		const char_t* const buf1 = this->_get_raw();
		const char_t* const buf2 = compare._get_raw();
		const bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;
		const size_t last = length - compare._len;

		size_t compares = 0;
		size_t i;
		for (i = indexOf; i < length;)
		{
			char_t lc = _safe_get(buf1, length, i, lowerCase);
			char_t rc = _safe_get(buf2, compare._len, compares, lowerCase);

			if (lc == rc)
			{
				++compares;
				if (compares == compare._len)
				{
					++i;
					break;
				}
			}
			else
			{
				if (i > last)
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

		if (compares == compare._len)
		{
			return i - compare._len;
		}
		else
		{
			return -1;
		}
	}

	[[nodiscard]] constexpr bool StartsWith(const String& compare, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		if (_len == 0)
		{
			return false;
		}

		if (!compare)
		{
			return false;
		}

		if (compare._len > _len)
		{
			return false;
		}

		const char_t* const buf1 = this->_get_raw();
		const char_t* const buf2 = compare._get_raw();
		const bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;

		size_t compares = 0;
		for (size_t i = 0; i < compare._len; ++i)
		{
			char_t lc = _safe_get(buf1, _len, i, lowerCase);
			char_t rc = _safe_get(buf2, compare._len, compares, lowerCase);

			if (lc == rc)
			{
				++compares;
				if (compares == compare._len)
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

	[[nodiscard]] constexpr bool EndsWith(const String& compare, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		if (_len == 0)
		{
			return false;
		}

		if (!compare)
		{
			return false;
		}

		if (compare._len > _len)
		{
			return false;
		}

		const char_t* const buf1 = this->_get_raw();
		const char_t* const buf2 = compare._get_raw();
		const bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;
		const size_t end = _len - compare._len;

		size_t compares = 0;
		for (size_t i = _len - 1; i >= end && i != -1; --i)
		{
			char_t lc = _safe_get(buf1, _len, i, lowerCase);
			char_t rc = _safe_get(buf2, compare._len, compare._len - (compares + 1), lowerCase);

			if (lc == rc)
			{
				++compares;
				if (compares == compare._len)
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

	template<IArray<char_t> TArray>
	[[nodiscard]] inline constexpr size_t IndexOfAny(const TArray& chars, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOfAny(chars, 0, comparison);
	}

	template<IArray<char_t> TArray>
	[[nodiscard]] constexpr size_t IndexOfAny(const TArray& chars, size_t indexOf, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		if (_len == 0)
		{
			return -1;
		}

		if (std::size(chars) == 0)
		{
			return -1;
		}

		const char_t* const buf = this->_get_raw();
		const bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;

		for (size_t i = indexOf; i < _len; ++i)
		{
			char_t c = _safe_get(buf, _len, i, lowerCase);

			bool found = false;
			if (lowerCase)
			{
				found = Linq::Contains(&chars, [&c](char_t wc)
				{
					return c == (char_t)std::tolower(wc);
				});
			}
			else
			{
				found = Linq::Contains(&chars, c);
			}

			if (found)
			{
				return i;
			}
		}

		return -1;
	}

	template<IArray<String> TArray>
	[[nodiscard]] constexpr size_t IndexOfAny(const TArray& strings, size_t indexOf, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		if (_len == 0)
		{
			return -1;
		}

		if (std::size(strings) == 0)
		{
			return -1;
		}

		std::vector<String> strs = Linq::ToVector(strings);
		std::vector<char_t> firsts = Linq::Select(std::ref(strs), [](const auto& str) { return str[0]; });
		const bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;

		for (size_t i = IndexOfAny(firsts, indexOf, comparison); i != -1; i = IndexOfAny(firsts, ++i, comparison))
		{
			String& candidate = strs[IndexOf(firsts, this->operator [](i))];

			std::wstring_view lhs(candidate._get_raw(), candidate._len);
			size_t sublen = _len - i;
			std::wstring_view rhs(this->_get_raw() + i, std::min(sublen, candidate._len));

			if (_compare_string_view(lhs, rhs, lowerCase))
			{
				return i;
			}
		}

		return -1;
	}

	[[nodiscard]] inline constexpr bool IsEmpty() const noexcept
	{
		return _len == 0;
	}

	[[nodiscard]] inline String Trim() const
	{
		return Trim(_whitespace_chars);
	}

	template<class TArray>
	[[nodiscard]] inline String Trim(const TArray& chars) const requires
		IArray<TArray, char_t>
	{
		return this->_trim_all((ETrimType)((int32)ETrimType::Head | (int32)ETrimType::Tail), chars);
	}

	[[nodiscard]] inline String TrimStart() const
	{
		return TrimStart(_whitespace_chars);
	}

	template<IArray<char_t> TArray>
	[[nodiscard]] inline String TrimStart(const TArray& chars) const
	{
		return this->_trim_all(ETrimType::Head, chars);
	}

	[[nodiscard]] inline String TrimEnd() const
	{
		return TrimEnd(_whitespace_chars);
	}

	template<IArray<char_t> TArray>
	[[nodiscard]] inline String TrimEnd(const TArray& chars) const
	{
		return this->_trim_all(ETrimType::Head, chars);
	}

	[[nodiscard]] String Substring(size_t start, size_t length = -1) const
	{
		if (start >= _len)
		{
			return String();
		}

		length = std::min(length, _len - start);
		return String(std::wstring_view(this->_get_raw() + start, length));
	}

	[[nodiscard]] constexpr std::wstring_view SubstringView(size_t start, size_t length = -1) const noexcept
	{
		if (start >= _len)
		{
			return std::wstring_view();
		}

		if (length > _len - start)
		{
			length = _len - start;
		}

		length = std::min(length, _len - start);
		return std::wstring_view(this->_get_raw() + start, length);
	}

	[[nodiscard]] inline std::vector<String> Split(char_t separator, EStringSplitOptions options = EStringSplitOptions::None) const
	{
		std::array<char_t, 1> seps{ separator };
		return Split(seps, options);
	}

	template<IArray<char_t> TArray>
	[[nodiscard]] std::vector<String> Split(const TArray& separators, EStringSplitOptions options = EStringSplitOptions::None) const
	{
		std::vector<String> results;

		const bool removeEmpty = (int32)(options & EStringSplitOptions::RemoveEmptyEntries) > 0;
		const bool trim = (int32)(options & EStringSplitOptions::TrimEntries) > 0;

		for (size_t i = 0; i < _len;)
		{
			std::optional<String> view;
			size_t seekp = IndexOfAny(separators, i);

			if (seekp == -1)
			{
				// Could not found separator.
				view = Substring(i);
				i = -1;
			}
			else
			{
				size_t length = seekp - i;
				if (length != 0 || !removeEmpty)
				{
					view = Substring(i, length);
				}
				i = seekp + 1;
			}

			if (view)
			{
				if (trim)
				{
					view = view->Trim();
					if (removeEmpty && view->_len == 0)
					{
						continue;
					}
				}

				results.emplace_back(view.value());
			}
		}

		return results;
	}

	[[nodiscard]] std::vector<String> Split(const String& separator, EStringSplitOptions options = EStringSplitOptions::None) const
	{
		std::vector<String> results;

		const bool removeEmpty = (int32)(options & EStringSplitOptions::RemoveEmptyEntries) > 0;
		const bool trim = (int32)(options & EStringSplitOptions::TrimEntries) > 0;

		for (size_t i = 0; i < _len;)
		{
			std::optional<String> view;
			size_t seekp = IndexOf(separator, i);

			if (seekp == -1)
			{
				// Could not found separator.
				view = Substring(i);
				i = -1;
			}
			else
			{
				size_t length = seekp - i;
				if (length != 0 || !removeEmpty)
				{
					view = Substring(i, length);
				}
				i = seekp + separator._len;
			}

			if (view)
			{
				if (trim)
				{
					view = Trim(view.value());
					if (removeEmpty && view->_len == 0)
					{
						continue;
					}
				}

				results.emplace_back(view.value());
			}
		}

		return results;
	}

	template<class TOp>
	[[nodiscard]] String Transform(TOp&& op) const requires
		std::convertible_to<std::invoke_result_t<TOp, char_t>, char_t>
	{
		decltype(_buf) nbuf;
		auto& ptr = nbuf.emplace<1>(std::make_shared<char_t[]>(_len + 1));
		const char_t* const buf = this->_get_raw();

		for (size_t i = 0; i < _len; ++i)
		{
			ptr[i] = op(buf[i]);
		}

		String s;
		s._buf = std::move(nbuf);
		s._len = _len;
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

	[[nodiscard]] String Replace(const String& oldValue, const String& newValue, EStringComparison comparison = EStringComparison::CurrentCulture) const
	{
		const char_t* const buf = this->_get_raw();

		std::vector<std::wstring_view> concat;

		for (size_t i = 0;;)
		{
			size_t indexOf = IndexOf(oldValue, i, comparison);
			if (indexOf == -1)
			{
				size_t len = _len - i;
				concat.emplace_back(std::wstring_view(buf + i, len));
				break;
			}
			else
			{
				size_t len = indexOf - i;
				concat.emplace_back(std::wstring_view(buf + i, len));
				concat.emplace_back(std::wstring_view(newValue._get_raw(), newValue._len));
				i = indexOf + oldValue._len;
			}
		}

		return _concat(concat);
	}

	[[nodiscard]] String ReplaceAt(size_t indexOf, size_t length, String newValue) const
	{
		if (!newValue)
		{
			return *this;
		}

		if (indexOf >= _len)
		{
			return *this;
		}

		length = std::min(length, _len - indexOf);
		size_t tail = indexOf + length;

		std::array<std::wstring_view, 3> concat;
		concat[0] = std::wstring_view(this->_get_raw(), indexOf);
		concat[1] = std::wstring_view(newValue._get_raw(), newValue._len);
		concat[2] = std::wstring_view(this->_get_raw() + tail, _len - tail);
		return _concat(concat);
	}

	[[nodiscard]] inline String Insert(size_t indexOf, String newValue) const
	{
		return ReplaceAt(indexOf, 0, newValue);
	}

	[[nodiscard]] inline String Insert(size_t indexOf, char_t ch) const
	{
		char_t buf[2] = { ch, 0 };
		return Insert(indexOf, String::FromLiteral(buf));
	}

	[[nodiscard]] inline String Remove(size_t indexOf, size_t length = -1) const
	{
		const size_t rem = _len - indexOf;
		if (length > rem || length == -1)
		{
			length = rem;
		}

		std::array<std::wstring_view, 2> concat;
		concat[0] = std::wstring_view(this->_get_raw(), indexOf);
		size_t app = indexOf + length;
		concat[1] = std::wstring_view(this->_get_raw() + app, _len - app);
		return _concat(concat);
	}

	[[nodiscard]] inline String Quotes(char_t c = '"') const
	{
		std::array<std::wstring_view, 3> concat;
		concat[0] = std::wstring_view(&c, 1);
		concat[1] = std::wstring_view(this->_get_raw(), _len);
		concat[2] = std::wstring_view(&c, 1);
		return _concat(concat);
	}

public:
	[[nodiscard]] static inline constexpr String FromLiteral(std::wstring_view str)
	{
		return String(decltype(_buf)(str.data()), str.length());
	}

	[[nodiscard]] static inline String FromLiteral(std::string_view str)
	{
		static thread_local std::unordered_map<const char*, String> views;
		if (auto it = views.find(str.data()); it != views.end())
		{
			return it->second;
		}
		else
		{
			String cv = String(str);
			views.emplace(str.data(), cv);
			return cv;
		}
	}

public:
	[[nodiscard]] static inline String FromCodepage(std::string_view s, int32 codepage = 0)
	{
		return String(PlatformMisc::FromCodepage(s, codepage));
	}

	[[nodiscard]] inline std::string AsCodepage(int32 codepage = 0) const
	{
		return PlatformMisc::AsCodepage(*this, codepage);
	}

public:
	template<class... TArgs>
	[[nodiscard]] static String Format(String formatStr, TArgs&&... args)
	{
		return String(std::vformat((std::wstring_view)formatStr, std::make_wformat_args(std::forward<TArgs>(args)...)));
	}

	template<IEnumerable<String> TEnumerable>
	[[nodiscard]] static String Concat(const TEnumerable& strings)
	{
		std::vector<std::wstring_view> buffers;
		for (const String& item : strings)
		{
			buffers.emplace_back(std::wstring_view(item._get_raw(), item._len));
		}

		return _concat(buffers);
	}

	template<class... TStrings>
	[[nodiscard]] static inline String Concat(TStrings&&... strings) requires CompatibleVariadic<String, TStrings...>
	{
		return Concat(std::array<String, sizeof...(TStrings)>({ (String)std::forward<TStrings>(strings)... }));
	}

	template<IEnumerable<String> TStrings>
	[[nodiscard]] static String Join(const String& separator, const TStrings& strings)
	{
		std::vector<String> concat;
		for (auto& item : strings)
		{
			if (concat.size() > 0)
			{
				concat.emplace_back(separator);
			}

			concat.emplace_back(item);
		}

		return Concat(concat);
	}

	template<class... TStrings>
	[[nodiscard]] static inline String Join(const String& separator, TStrings&&... strings) requires CompatibleVariadic<String, TStrings...>
	{
		return Join(separator, std::array<String, sizeof...(TStrings)>({ (String)std::forward<TStrings>(strings)... }));
	}

	[[nodiscard]] static String GetPlaceholder(int32 number, String formatArgs)
	{
		if (!formatArgs)
		{
			return Format(FromLiteral(L"{{{}}}"), number);
		}
		else
		{
			return Format(FromLiteral(L"{{{}:{}}}"), number, formatArgs);
		}
	}

	[[nodiscard]] static String GetPlaceholder(const String& formatArgs)
	{
		if (!formatArgs)
		{
			return FromLiteral(L"{}");
		}
		else
		{
			return Format(FromLiteral(L"{{:{}}}"), formatArgs);
		}
	}

	[[nodiscard]] static inline constexpr String GetEmpty();

	[[noreturn]]
	static constexpr void ThrowFormatError(bool);
	[[noreturn]]
	static constexpr void ThrowArgumentError(bool);

	template<class T = int32, IArray<char_t> IString>
	static constexpr T Stoi(const IString& str, bool hex = false) requires std::integral<T>
	{
		T number = 0;

		for (size_t i = 0; i < str.length(); ++i)
		{
			char_t ch = Array<>::At(str, i);
			T value = 0;
			if (ch >= '0' && ch <= '9')
			{
				value = (T)(ch - '0');
			}
			else if (ch >= 'A' && ch <= 'F' && hex)
			{
				value = (T)10 + (T)(ch - 'A');
			}
			else if (ch >= 'a' && ch <= 'f' && hex)
			{
				value = (T)10 + (T)(ch - 'a');
			}
			else
			{
				ThrowFormatError(true);
			}

			number *= hex ? (T)16 : (T)10;
			number += value;
		}

		return number;
	}

	template<class TChar = char_t>
	static constexpr TChar HexChar(unsigned char number, bool bUpper = true)
	{
		if (number < 10)
		{
			return (TChar)('0' + number);
		}
		else if (number < 16)
		{
			return (TChar)((bUpper ? 'A' : 'a') + (number - 10));
		}
		else
		{
			ThrowArgumentError(true);
		}
	}

	template<class TChar>
	static constexpr unsigned char CharHex(TChar ch)
	{
		if (ch >= '0' && ch <= '9')
		{
			return (unsigned char)(ch - '0');
		}
		else if (ch >= 'a' && ch <= 'z')
		{
			return (unsigned char)(ch - 'a') + 10;
		}
		else if (ch >= 'A' && ch <= 'Z')
		{
			return (unsigned char)(ch - 'A') + 10;
		}
		else
		{
			ThrowArgumentError(true);
		}
	}

	template<class T, class TChar>
	static void Hex(T number, TChar* dest) noexcept
	{
		for (size_t i = 0; i < sizeof(T); ++i)
		{
			unsigned char slot = (unsigned char)number;
			number >>= 8;
			dest[i * 2 + 0] = HexChar<TChar>(slot / 16);
			dest[i * 2 + 1] = HexChar<TChar>(slot % 16);
		}
	}

	template<class TChar>
	static constexpr bool IsHex(TChar ch) noexcept
	{
		return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z');
	}
};

namespace StringInternal
{
	constexpr inline String EmptyStr;
}

[[nodiscard]]
inline constexpr String String::GetEmpty()
{
	return StringInternal::EmptyStr;
}

[[nodiscard]]
inline constexpr char_t operator ""_c(char Val)
{
	return (char_t)Val;
}

[[nodiscard]]
inline constexpr char_t operator ""_c(wchar_t Val)
{
	return (char_t)Val;
}

template<>
struct std::formatter<String, char_t> : public std::formatter<std::wstring_view, char_t>
{
	template<class TFormatContext>
	auto format(const String& str, TFormatContext& ctx)
	{
		return std::formatter<std::wstring_view, char_t>::format((std::wstring_view)str, ctx);
	}
};

template<>
struct std::formatter<String, char> : public std::formatter<std::string_view, char>
{
	template<class TFormatContext>
	auto format(const String& str, TFormatContext& ctx)
	{
		return std::formatter<std::string_view, char>::format((std::string)str, ctx);
	}
};

template<lib::details::_to_string T, class TChar>
struct std::formatter<T, TChar> : public std::formatter<String, TChar>
{
	template<class U, class TFormatContext>
	auto format(U&& obj, TFormatContext& ctx)
	{
		return std::formatter<String, TChar>::format(Misc::ToRef(std::forward<U>(obj)).ToString(), ctx);
	}
};

#define TEXT(x) String::FromLiteral(L ## x)