// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "PrimitiveTypes.h"
#include "StringComparison.h"
#include "StringSplitOptions.h"
#include "Linq/Array.h"
#include "Concepts/CompatibleVariadic.h"
#include "Generic/IObject.h"
#include "Misc/Misc.h"
#include <variant>
#include <array>
#include <memory>
#include <string_view>
#include <utility>
#include <compare>
#include <format>
#include <vector>

class CORE_API String : public ArrayExtensions<wchar_t>
{
	std::variant<const wchar_t*, std::shared_ptr<wchar_t[]>> _buf;
	size_t _len;

private:
	static inline constexpr const wchar_t _Null_char = 0;
	static inline constexpr const std::array _Whitespace_chars = { L' ', L'\t', L'\r', L'\n', L'\b' };

private:
	inline constexpr String(decltype(_buf) buf, size_t len)
		: _buf(std::move(buf))
		, _len(len)
	{
	}

	inline constexpr const wchar_t* _Get_raw() const noexcept
	{
		const wchar_t* buf = nullptr;
		switch (_buf.index())
		{
		case 0:
			buf = std::get<0>(_buf);
			break;
		case 1:
			buf = std::get<1>(_buf).get();
			break;
		}

		return buf ? buf : &_Null_char;
	}

	static inline constexpr wchar_t _Safe_get(const wchar_t* buf, size_t len, size_t idx, bool lowerCase = false) noexcept
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

	inline constexpr std::strong_ordering _Compare_to(const wchar_t* buf, size_t len, EStringComparison comparison) const noexcept
	{
		size_t length = std::min(_len, len) + 1;
		const wchar_t* mybuf = this->_Get_raw();

		bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;

		for (size_t i = 0; i < length; ++i)
		{
			auto cmp = _Safe_get(mybuf, _len, i, lowerCase) <=> _Safe_get(buf, len, i, lowerCase);
			if (cmp != 0)
			{
				return cmp;
			}
		}

		return std::strong_ordering::equal;
	}

	inline static constexpr bool _Compare_string_view(std::wstring_view lhs, std::wstring_view rhs, bool lowerCase)
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

	String& _Allocate_assign(const wchar_t* buf, size_t len);
	static std::wstring _As_unicode(std::string_view s, int32 codepage);
	static std::string _As_multibyte(std::wstring_view s, int32 codepage);

	enum class ETrimType
	{
		Head = 0x1,
		Tail = 0x2,
	};

	template<IArray<wchar_t> TArray>
	constexpr String _Trim_all(ETrimType tt, const TArray& chars) const
	{
		if (_len == 0)
		{
			return String();
		}

		const wchar_t* const buf = _Get_raw();

		size_t head = 0;
		if (((int32)tt & (int32)ETrimType::Head) > 0)
		{
			for (; head < _len; ++head)
			{
				wchar_t wc = _Safe_get(buf, _len, head);
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
				wchar_t wc = _Safe_get(buf, _len, tail - 1);
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
	static String _Concat(const TArray& concat)
	{
		size_t length = 0;
		for (const std::wstring_view& item : concat)
		{
			length += item.length();
		}

		decltype(_buf) nbuf;
		auto& ptr = nbuf.emplace<1>(std::make_shared<wchar_t[]>(length + 1));
		size_t appendIdx = 0;
		for (const std::wstring_view& item : concat)
		{
			memcpy(ptr.get() + appendIdx, item.data(), sizeof(wchar_t) * item.length());
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
		std::same_as<T, wchar_t>
		: String(&ch, 1)
	{
	}

	explicit inline String(std::string_view s)
		: String(_As_unicode(s, 0))
	{
	}

	inline String(const char* buf, size_t len)
		: String(_As_unicode(std::string_view(buf, len), 0))
	{
	}

	explicit inline String(std::wstring_view s)
	{
		this->_Allocate_assign(s.data(), s.length());
	}

	inline String(const wchar_t* buf, size_t len)
		: String(std::wstring_view(buf, len))
	{
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

	inline constexpr const wchar_t& operator [](size_t idx) const noexcept
	{
		const wchar_t* buf = this->_Get_raw();
		if (buf != nullptr)
		{
			return buf[idx];
		}
		return _Null_char;
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
		return this->_Compare_to(rhs._Get_raw(), rhs._len, EStringComparison::CurrentCulture);
	}

	inline String operator +(const String& rhs)
	{
		return Concat(*this, rhs);
	}

	inline String& operator +=(const String& rhs)
	{
		return this->operator =(*this + rhs);
	}

	inline explicit operator std::string() const noexcept
	{
		return _As_multibyte(std::wstring_view(this->_Get_raw(), _len), 0);
	}

	inline explicit operator std::wstring() const noexcept
	{
		return std::wstring(this->_Get_raw(), _len);
	}

	inline constexpr operator std::wstring_view() const noexcept
	{
		return std::wstring_view(this->_Get_raw(), _len);
	}

	inline constexpr explicit operator const wchar_t* () const noexcept
	{
		return this->_Get_raw();
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
#pragma region IEnumerable<wchar_t>
	inline constexpr const wchar_t* begin() const noexcept
	{
		if (_len == 0)
		{
			return &_Null_char;
		}
		else
		{
			return _Get_raw();
		}
	}

	inline constexpr const wchar_t* end() const noexcept
	{
		if (_len == 0)
		{
			return &_Null_char;
		}
		else
		{
			return _Get_raw() + _len;
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
		return this->_Compare_to(rhs._Get_raw(), rhs._len, EStringComparison::CurrentCulture);
	}
#pragma endregion

public:
	[[nodiscard]] inline constexpr bool Equals(const String& rhs, EStringComparison comparison) const noexcept
	{
		if (_len != rhs._len)
		{
			return false;
		}

		return this->_Compare_to(rhs._Get_raw(), rhs._len, comparison) == 0;
	}

	[[nodiscard]] inline constexpr String ToString() const noexcept
	{
		return *this;
	}

	[[nodiscard]] inline constexpr size_t length() const noexcept
	{
		return (size_t)*this;
	}

	[[nodiscard]] inline constexpr const wchar_t* c_str() const noexcept
	{
		return (const wchar_t*)*this;
	}

	[[nodiscard]] constexpr std::strong_ordering CompareTo(const String& rhs, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return this->_Compare_to(rhs._Get_raw(), rhs._len, comparison);
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

	[[nodiscard]] inline constexpr size_t IndexOf(wchar_t ch, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(ch, 0, -1, comparison);
	}

	[[nodiscard]] inline constexpr size_t IndexOf(wchar_t ch, size_t indexOf, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(ch, indexOf, -1, comparison);
	}

	[[nodiscard]] constexpr size_t IndexOf(wchar_t ch, size_t indexOf, size_t length, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
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

		const wchar_t* const buf1 = this->_Get_raw();
		const bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;

		if (lowerCase)
		{
			ch = std::tolower(ch);
		}

		for (size_t i = indexOf; i < length; ++i)
		{
			wchar_t lc = _Safe_get(buf1, length, i, lowerCase);
			wchar_t rc = ch;

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

		const wchar_t* const buf1 = this->_Get_raw();
		const wchar_t* const buf2 = compare._Get_raw();
		const bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;
		const size_t last = length - compare._len;

		size_t compares = 0;
		size_t i;
		for (i = indexOf; i < length; ++i)
		{
			wchar_t lc = _Safe_get(buf1, length, i, lowerCase);
			wchar_t rc = _Safe_get(buf2, compare._len, compares, lowerCase);

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
				compares = 0;
				if (i > last)
				{
					break;
				}
			}
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

		const wchar_t* const buf1 = this->_Get_raw();
		const wchar_t* const buf2 = compare._Get_raw();
		const bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;

		size_t compares = 0;
		for (size_t i = 0; i < compare._len; ++i)
		{
			wchar_t lc = _Safe_get(buf1, _len, i, lowerCase);
			wchar_t rc = _Safe_get(buf2, compare._len, compares, lowerCase);

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

		const wchar_t* const buf1 = this->_Get_raw();
		const wchar_t* const buf2 = compare._Get_raw();
		const bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;
		const size_t end = _len - compare._len;

		size_t compares = 0;
		for (size_t i = _len - 1; i >= end && i != -1; --i)
		{
			wchar_t lc = _Safe_get(buf1, _len, i, lowerCase);
			wchar_t rc = _Safe_get(buf2, compare._len, compare._len - (compares + 1), lowerCase);

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

	template<IArray<wchar_t> TArray>
	[[nodiscard]] inline constexpr size_t IndexOfAny(const TArray& chars, EStringComparison comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOfAny(chars, 0, comparison);
	}

	template<IArray<wchar_t> TArray>
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

		const wchar_t* const buf = this->_Get_raw();
		const bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;

		for (size_t i = indexOf; i < _len; ++i)
		{
			wchar_t c = _Safe_get(buf, _len, i, lowerCase);

			bool found = false;
			if (lowerCase)
			{
				found = Linq::Contains(&chars, [&c](wchar_t wc)
				{
					return c == (wchar_t)std::tolower(wc);
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
		std::vector<wchar_t> firsts = Linq::Select(std::ref(strs), [](const auto& str) { return str[0]; });
		const bool lowerCase = comparison == EStringComparison::CurrentCultureIgnoreCase;

		for (size_t i = IndexOfAny(firsts, indexOf, comparison); i != -1; i = IndexOfAny(firsts, ++i, comparison))
		{
			String& candidate = strs[IndexOf(firsts, this->operator [](i))];

			std::wstring_view lhs(candidate._Get_raw(), candidate._len);
			size_t sublen = _len - i;
			std::wstring_view rhs(this->_Get_raw() + i, std::min(sublen, candidate._len));

			if (_Compare_string_view(lhs, rhs, lowerCase))
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
		return Trim(_Whitespace_chars);
	}

	template<class TArray>
	[[nodiscard]] inline String Trim(const TArray& chars) const requires
		IArray<TArray, wchar_t>
	{
		return this->_Trim_all((ETrimType)((int32)ETrimType::Head | (int32)ETrimType::Tail), chars);
	}

	[[nodiscard]] inline String TrimStart() const
	{
		return TrimStart(_Whitespace_chars);
	}

	template<IArray<wchar_t> TArray>
	[[nodiscard]] inline String TrimStart(const TArray& chars) const
	{
		return this->_Trim_all(ETrimType::Head, chars);
	}

	[[nodiscard]] inline String TrimEnd() const
	{
		return TrimEnd(_Whitespace_chars);
	}

	template<IArray<wchar_t> TArray>
	[[nodiscard]] inline String TrimEnd(const TArray& chars) const
	{
		return this->_Trim_all(ETrimType::Head, chars);
	}

	[[nodiscard]] String Substring(size_t start, size_t length = -1) const
	{
		if (start >= _len)
		{
			return String();
		}

		length = std::min(length, _len - start);
		return String(std::wstring_view(this->_Get_raw() + start, length));
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
		return std::wstring_view(this->_Get_raw() + start, length);
	}

	[[nodiscard]] inline std::vector<String> Split(wchar_t separator, EStringSplitOptions options = EStringSplitOptions::None) const
	{
		std::array<wchar_t, 1> seps{ separator };
		return Split(seps, options);
	}

	template<IArray<wchar_t> TArray>
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
			size_t seekp = IndexOf(separator);

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
		std::convertible_to<std::invoke_result_t<TOp, wchar_t>, wchar_t>
	{
		decltype(_buf) nbuf;
		auto& ptr = nbuf.emplace<1>(std::make_shared<wchar_t[]>(_len + 1));
		const wchar_t* const buf = this->_Get_raw();

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
		const wchar_t* const buf = this->_Get_raw();

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
				concat.emplace_back(std::wstring_view(newValue._Get_raw(), newValue._len));
				i = indexOf + oldValue._len;
			}
		}

		return _Concat(concat);
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
		concat[0] = std::wstring_view(this->_Get_raw(), indexOf);
		concat[1] = std::wstring_view(newValue._Get_raw(), newValue._len);
		concat[2] = std::wstring_view(this->_Get_raw() + tail, _len - tail);
		return _Concat(concat);
	}

	[[nodiscard]] inline String Insert(size_t indexOf, String newValue) const
	{
		return ReplaceAt(indexOf, 0, newValue);
	}

	[[nodiscard]] inline String Insert(size_t indexOf, wchar_t ch) const
	{
		wchar_t buf[2] = { ch, 0 };
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
		concat[0] = std::wstring_view(this->_Get_raw(), indexOf);
		size_t app = indexOf + length;
		concat[1] = std::wstring_view(this->_Get_raw() + app, _len - app);
		return _Concat(concat);
	}

public:
	[[nodiscard]] static inline constexpr String FromLiteral(std::wstring_view str)
	{
		return String(decltype(_buf)(str.data()), str.length());
	}

	[[nodiscard]] static inline String FromLiteral(std::string_view str)
	{
		return String(str);
	}

public:
	[[nodiscard]] static inline String FromCodepage(std::string_view s, int32 codepage)
	{
		return String(_As_unicode(s, codepage));
	}

	[[nodiscard]] inline std::string AsCodepage(int32 codepage) const
	{
		return _As_multibyte(*this, codepage);
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
			buffers.emplace_back(std::wstring_view(item._Get_raw(), item._len));
		}

		return _Concat(buffers);
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

	[[nodiscard]] static String GetPlaceholder(String formatArgs)
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
};

inline constexpr auto operator "" _s(const wchar_t* lit, size_t len)
{
	return String::FromLiteral(std::wstring_view(lit, len));
}

inline String operator "" _s(const char* lit, size_t len)
{
	return String::FromLiteral(std::string_view(lit, len));
}

inline constexpr wchar_t operator "" _s(wchar_t wc)
{
	return wc;
}

inline constexpr char operator "" _s(char c)
{
	return (wchar_t)c;
}

template<>
struct std::formatter<String, wchar_t> : public std::formatter<std::wstring_view, wchar_t>
{
	template<class TFormatContext>
	auto format(const String& str, TFormatContext& ctx)
	{
		return std::formatter<std::wstring_view, wchar_t>::format((std::wstring_view)str, ctx);
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

template<IObject T, class TChar>
struct std::formatter<T, TChar> : public std::formatter<String, TChar>
{
	template<class U, class TFormatContext>
	auto format(U&& obj, TFormatContext& ctx)
	{
		return std::formatter<String, TChar>::format(Misc::ToRef(std::forward<U>(obj)).ToString(), ctx);
	}
};

#define TEXT(x) L ## x ## _s