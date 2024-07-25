// Copyright 2020-2024 Aumoa.lib. All right reserved.

module;

#include "Platform/PlatformMacros.h"

export module Core:String;

export import :Std;
export import :Char;
export import :IntegralTypes;
export import :StringComparison;
export import :StringSplitOptions;
export import :Math;
export import :Contains;

export class CORE_API String
{
	using Buf_t = std::variant<std::basic_string_view<char_t>, std::shared_ptr<char_t[]>>;
	Buf_t Buf;
	size_t Len;
	bool bNullTerminate;

private:
	// FromLiteral specialized.
	inline consteval String(std::basic_string_view<char_t> Str, size_t Len, std::in_place_t) noexcept
		: Buf(std::move(Str))
		, Len(Len)
		, bNullTerminate(true)
	{
	}

	inline constexpr String(Buf_t&& Buf, size_t Len)
		: Buf(std::move(Buf))
		, Len(Len)
		, bNullTerminate(true)
	{
	}

	inline constexpr const char_t* GetRaw() const noexcept
	{
		const char_t* Ptr = nullptr;
		switch (Buf.index())
		{
		case 0:
			Ptr = std::get<0>(Buf).data();
			break;
		case 1:
			Ptr = std::get<1>(Buf).get();
			break;
		}

		return Ptr ? Ptr : &Char::k_NullChar;
	}

	inline constexpr const std::basic_string_view<char_t> GetEval() const noexcept
	{
		return std::get<0>(Buf);
	}

	static inline constexpr char_t SafeGet(const char_t* InBuf, size_t InLen, size_t Idx, bool bLowerCase = false) noexcept
	{
		if (Idx >= InLen || InBuf == nullptr)
		{
			return 0;
		}
		else
		{
			return bLowerCase ? Char::ToLower(InBuf[Idx]) : InBuf[Idx];
		}
	}

	inline constexpr std::strong_ordering InternalCompareTo(const char_t* InBuf, size_t InLen, EStringComparison Comparison) const noexcept
	{
		size_t Length = Math::Min(Len, InLen) + 1;
		const char_t* MyBuf = GetRaw();

		bool bLowerCase = Comparison == EStringComparison::CurrentCultureIgnoreCase;

		for (size_t i = 0; i < Length; ++i)
		{
			auto Cmp = SafeGet(MyBuf, Len, i, bLowerCase) <=> SafeGet(InBuf, InLen, i, bLowerCase);
			if (Cmp != 0)
			{
				return Cmp;
			}
		}

		return std::strong_ordering::equal;
	}

	String& AllocateAssign(const char_t* InBuf, size_t InLen);

	enum class ETrimType
	{
		Head = 0x1,
		Tail = 0x2,
	};

	template<std::ranges::input_range T> requires std::convertible_to<std::ranges::range_value_t<T>, char_t>
	String TrimAll(ETrimType TrimType, const T& Chars) const
	{
		if (Len == 0)
		{
			return String();
		}

		const char_t* const MyBuf = GetRaw();

		size_t Head = 0;
		if (((int32)TrimType & (int32)ETrimType::Head) > 0)
		{
			for (; Head < Len; ++Head)
			{
				char_t Wc = SafeGet(MyBuf, Len, Head);
				if ((Chars | Linq::Contains(Wc)) == false)
				{
					break;
				}
			}
		}

		if (Head >= Len)
		{
			return String();
		}

		size_t Tail = Len;
		if (((int32)TrimType & (int32)ETrimType::Tail) > 0)
		{
			for (; Tail > Head; --Tail)
			{
				char_t Wc = SafeGet(MyBuf, Len, Tail - 1);
				if ((Chars | Linq::Contains(Wc)) == false)
				{
					break;
				}
			}
		}

		if (Tail <= Head)
		{
			return String();
		}

		size_t Span = Tail - Head;
		return Substring(Head, Span);
	}

	template<std::ranges::input_range T> requires std::convertible_to<std::ranges::range_value_t<T>, std::wstring_view>
	static String InternalConcat(const T& InElements)
	{
		size_t Length = 0;
		for (const std::wstring_view& Elem : InElements)
		{
			Length += Elem.length();
		}

		decltype(Buf) Nbuf;
		auto& Ptr = Nbuf.emplace<1>(std::make_shared<char_t[]>(Length + 1));
		size_t AppendIdx = 0;
		for (const std::wstring_view& Elem : InElements)
		{
			memcpy(Ptr.get() + AppendIdx, Elem.data(), sizeof(char_t) * Elem.length());
			AppendIdx += Elem.length();
		}

		Ptr[Length] = 0;
		return String(std::move(Nbuf), Length);
	}

public:
	inline constexpr String() noexcept
		: Buf(&Char::k_NullChar)
		, Len(0)
		, bNullTerminate(true)
	{
	}

	inline constexpr String(const String& Rhs) noexcept
		: Buf(Rhs.Buf)
		, Len(Rhs.Len)
		, bNullTerminate(Rhs.bNullTerminate)
	{
	}

	inline constexpr String(String&& Rhs) noexcept
		: Buf(std::move(Rhs.Buf))
		, Len(Rhs.Len)
		, bNullTerminate(Rhs.bNullTerminate)
	{
		Rhs.Buf = &Char::k_NullChar;
		Rhs.Len = 0;
		Rhs.bNullTerminate = true;
	}

	template<class T>
	explicit inline constexpr String(const T& Ch) noexcept requires
		std::same_as<T, char> ||
		std::same_as<T, char_t>
		: String(&Ch, 1)
	{
	}

	explicit String(std::string_view Str);
	String(const char* Buf, size_t Len);

	explicit inline String(std::wstring_view Sv)
	{
		this->AllocateAssign(Sv.data(), Sv.length());
	}

	inline String(const char_t* InBuf, size_t InLen)
		: String(std::wstring_view(InBuf, InLen))
	{
	}

	inline String(char_t Ch, size_t InLen)
	{
		auto& Ptr = Buf.emplace<1>(std::make_shared<char_t[]>(InLen + 1));
		auto* Rptr = Ptr.get();

		for (size_t i = 0; i < InLen; ++i)
		{
			Rptr[i] = Ch;
		}

		Rptr[InLen] = 0;
		Len = InLen;
		bNullTerminate = true;
	}

public:
	inline constexpr String& operator =(const String& Rhs) noexcept
	{
		Buf = Rhs.Buf;
		Len = Rhs.Len;
		bNullTerminate = Rhs.bNullTerminate;
		return *this;
	}

	inline constexpr String& operator =(String&& Rhs) noexcept
	{
		Buf = std::move(Rhs.Buf);
		Len = Rhs.Len;
		bNullTerminate = Rhs.bNullTerminate;
		Rhs.Buf = &Char::k_NullChar;
		Rhs.Len = 0;
		Rhs.bNullTerminate = true;
		return *this;
	}

	inline constexpr const char_t& operator [](size_t Idx) const noexcept
	{
		const char_t* MyBuf = this->GetRaw();
		if (MyBuf != nullptr)
		{
			return MyBuf[Idx];
		}
		return Char::k_NullChar;
	}

	inline constexpr bool operator ==(const String& Rhs) const noexcept
	{
		if (Len != Rhs.Len)
		{
			return false;
		}

		return this->operator <=>(Rhs) == 0;
	}

	inline constexpr std::strong_ordering operator <=>(const String& Rhs) const noexcept
	{
		return this->InternalCompareTo(Rhs.GetRaw(), Rhs.Len, EStringComparison::CurrentCulture);
	}

	inline String operator +(const String& Rhs) const
	{
		return Concat(*this, Rhs);
	}

	inline String& operator +=(const String& Rhs)
	{
		return this->operator =(*this + Rhs);
	}

	inline explicit operator std::string() const { return AsCodepage(); }
	inline std::string string() const { return (std::string)*this; }

	inline explicit operator std::wstring() const { return std::wstring(this->GetRaw(), Len); }
	inline std::wstring wstring() const { return (std::wstring)*this; }

	inline constexpr operator std::wstring_view() const noexcept { return std::wstring_view(this->GetRaw(), Len); }
	inline std::wstring_view wstring_view() const { return (std::wstring_view)*this; }

	inline explicit operator std::filesystem::path() const { return std::filesystem::path(this->GetRaw(), this->GetRaw() + Len); }
	inline std::filesystem::path path() const { return (std::filesystem::path)*this; }

#if !__INTELLISENSE__
	template<class... TArgs>
	inline constexpr operator std::basic_format_string<char_t, TArgs...>() const noexcept
	{
		return GetEval();
	}
#endif

	inline constexpr explicit operator const char_t* () const noexcept
	{
		return this->GetRaw();
	}

	inline constexpr explicit operator size_t () const noexcept
	{
		return Len;
	}

	inline constexpr explicit operator bool() const noexcept
	{
		return Len != 0;
	}

public:
#pragma region IEnumerable<char_t>
	inline constexpr const char_t* begin() const noexcept
	{
		if (Len == 0)
		{
			return &Char::k_NullChar;
		}
		else
		{
			return GetRaw();
		}
	}

	inline constexpr const char_t* end() const noexcept
	{
		if (Len == 0)
		{
			return &Char::k_NullChar;
		}
		else
		{
			return GetRaw() + Len;
		}
	}

	[[nodiscard]] inline constexpr size_t size() const noexcept
	{
		return Len + 1;
	}
#pragma endregion

#pragma region IEquatable<String>
public:
	[[nodiscard]] constexpr inline bool Equals(const String& Rhs) const noexcept
	{
		return Equals(Rhs, EStringComparison::CurrentCulture);
	}
#pragma endregion
#pragma region IComparable<String>
public:
	[[nodiscard]] constexpr std::strong_ordering CompareTo(const String& Rhs) const noexcept
	{
		return this->InternalCompareTo(Rhs.GetRaw(), Rhs.Len, EStringComparison::CurrentCulture);
	}
#pragma endregion

public:
	[[nodiscard]] inline constexpr bool Equals(const String& Rhs, EStringComparison Comparison) const noexcept
	{
		if (Len != Rhs.Len)
		{
			return false;
		}

		return this->InternalCompareTo(Rhs.GetRaw(), Rhs.Len, Comparison) == 0;
	}

	[[nodiscard]] inline constexpr String ToString() const noexcept
	{
		return *this;
	}

	[[nodiscard]] inline String Clone() const
	{
		return String(this->GetRaw(), this->Len);
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
		static thread_local std::basic_string<char_t> scoped;
		scoped = wstring_view();
		return scoped.c_str();
	}

	[[nodiscard]] constexpr std::strong_ordering CompareTo(const String& Rhs, EStringComparison Comparison) const noexcept
	{
		return this->InternalCompareTo(Rhs.GetRaw(), Rhs.Len, Comparison);
	}

	[[nodiscard]] inline constexpr bool Contains(const String& InCompare, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(InCompare, 0, (size_t)-1, Comparison) != -1;
	}

	[[nodiscard]] inline constexpr bool Contains(const String& InCompare, size_t InIndexOf, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(InCompare, InIndexOf, (size_t)-1, Comparison) != -1;
	}

	[[nodiscard]] inline constexpr bool Contains(const String& InCompare, size_t InIndexOf, size_t InLength, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(InCompare, InIndexOf, InLength, Comparison) != -1;
	}

	[[nodiscard]] inline constexpr size_t IndexOf(char_t InChar, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(InChar, 0, (size_t)-1, Comparison);
	}

	[[nodiscard]] inline constexpr size_t IndexOf(char_t InChar, size_t InIndexOf, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(InChar, InIndexOf, (size_t)-1, Comparison);
	}

	[[nodiscard]] constexpr size_t IndexOf(char_t InChar, size_t InIndexOf, size_t InLength, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		if (InLength > Len || InLength == -1)
		{
			InLength = Len;
		}

		if (InLength == 0)
		{
			return (size_t)-1;
		}

		if (Len == 0)
		{
			return (size_t)-1;
		}

		const char_t* const Buf1 = this->GetRaw();
		const bool bLowerCase = Comparison == EStringComparison::CurrentCultureIgnoreCase;

		if (bLowerCase)
		{
			InChar = (char_t)std::tolower(InChar);
		}

		for (size_t i = InIndexOf; i < InLength; ++i)
		{
			char_t Lch = SafeGet(Buf1, InLength, i, bLowerCase);
			char_t Rch = InChar;

			if (Lch == Rch)
			{
				return i;
			}
		}

		return (size_t)-1;
	}

	[[nodiscard]] inline constexpr size_t IndexOf(const String& InCompare, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(InCompare, 0, (size_t)-1, Comparison);
	}

	[[nodiscard]] inline constexpr size_t IndexOf(const String& InCompare, size_t InIndexOf, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOf(InCompare, InIndexOf, (size_t)-1, Comparison);
	}

	[[nodiscard]] constexpr size_t IndexOf(const String& InCompare, size_t InIndexOf, size_t InLength, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		if (InIndexOf > Len)
		{
			return (size_t)-1;
		}

		size_t MaxLength = Len - InIndexOf;
		if (InLength > MaxLength || InLength == -1)
		{
			InLength = MaxLength;
		}

		if (InLength == 0)
		{
			return (size_t)-1;
		}

		if (!InCompare)
		{
			return (size_t)-1;
		}

		if (InCompare.Len > InLength)
		{
			return (size_t)-1;
		}

		if (InIndexOf >= Len - InCompare.Len)
		{
			return (size_t)-1;
		}

		const char_t* const Buf1 = this->GetRaw();
		const char_t* const Buf2 = InCompare.GetRaw();
		const bool bLowerCase = Comparison == EStringComparison::CurrentCultureIgnoreCase;
		const size_t LastIndex = Len - InCompare.Len;

		size_t Compares = 0;
		size_t i;
		for (i = InIndexOf; i < Len;)
		{
			char_t Lch = SafeGet(Buf1, Len, i, bLowerCase);
			char_t Rch = SafeGet(Buf2, InCompare.Len, Compares, bLowerCase);

			if (Lch == Rch)
			{
				++Compares;
				if (Compares == InCompare.Len)
				{
					++i;
					break;
				}
			}
			else
			{
				if (i > LastIndex)
				{
					break;
				}

				if (Compares > 0)
				{
					Compares = 0;
					continue;
				}
			}

			++i;
		}

		if (Compares == InCompare.Len)
		{
			return i - InCompare.Len;
		}
		else
		{
			return (size_t)-1;
		}
	}

	[[nodiscard]] inline constexpr size_t LastIndexOf(char_t InChar, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return LastIndexOf(InChar, 0, (size_t)-1, Comparison);
	}

	[[nodiscard]] inline constexpr size_t LastIndexOf(char_t InChar, size_t InIndexOf, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return LastIndexOf(InChar, InIndexOf, (size_t)-1, Comparison);
	}

	[[nodiscard]] constexpr size_t LastIndexOf(char_t InChar, size_t InIndexOf, size_t InLength, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		if (InLength > Len || InLength == -1)
		{
			InLength = Len;
		}

		if (InLength == 0)
		{
			return (size_t)-1;
		}

		if (Len == 0)
		{
			return (size_t)-1;
		}

		const char_t* const Buf1 = this->GetRaw();
		const bool bLowerCase = Comparison == EStringComparison::CurrentCultureIgnoreCase;

		if (bLowerCase)
		{
			InChar = (char_t)std::tolower(InChar);
		}

		for (size_t i = InIndexOf; i < InLength; ++i)
		{
			size_t Index = InLength - i - 1;
			char_t Lch = SafeGet(Buf1, InLength, Index, bLowerCase);
			char_t Rch = InChar;

			if (Lch == Rch)
			{
				return Index;
			}
		}

		return (size_t)-1;
	}

	[[nodiscard]] constexpr bool StartsWith(const String& InCompare, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		if (Len == 0)
		{
			return false;
		}

		if (!InCompare)
		{
			return false;
		}

		if (InCompare.Len > Len)
		{
			return false;
		}

		const char_t* const Buf1 = this->GetRaw();
		const char_t* const Buf2 = InCompare.GetRaw();
		const bool bLowerCase = Comparison == EStringComparison::CurrentCultureIgnoreCase;

		size_t Compares = 0;
		for (size_t i = 0; i < InCompare.Len; ++i)
		{
			char_t Lch = SafeGet(Buf1, Len, i, bLowerCase);
			char_t Rch = SafeGet(Buf2, InCompare.Len, Compares, bLowerCase);

			if (Lch == Rch)
			{
				++Compares;
				if (Compares == InCompare.Len)
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

	[[nodiscard]] constexpr bool EndsWith(const String& InCompare, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		if (Len == 0)
		{
			return false;
		}

		if (!InCompare)
		{
			return false;
		}

		if (InCompare.Len > Len)
		{
			return false;
		}

		const char_t* const Buf1 = this->GetRaw();
		const char_t* const Buf2 = InCompare.GetRaw();
		const bool bLowerCase = Comparison == EStringComparison::CurrentCultureIgnoreCase;
		const size_t EndIndex = Len - InCompare.Len;

		size_t Compares = 0;
		for (size_t i = Len - 1; i >= EndIndex && i != -1; --i)
		{
			char_t Lch = SafeGet(Buf1, Len, i, bLowerCase);
			char_t Rch = SafeGet(Buf2, InCompare.Len, InCompare.Len - (Compares + 1), bLowerCase);

			if (Lch == Rch)
			{
				++Compares;
				if (Compares == InCompare.Len)
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
		return Buf.index() == 0;
	}

	[[nodiscard]] inline constexpr bool IsNullTerminate() const noexcept
	{
		return bNullTerminate;
	}

	template<std::ranges::input_range TCharArray>
		requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
	[[nodiscard]] inline constexpr size_t IndexOfAny(const TCharArray& Chars, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		return IndexOfAny(Chars, 0, Comparison);
	}

	template<std::ranges::input_range TCharArray>
		requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
	[[nodiscard]] constexpr size_t IndexOfAny(const TCharArray& Chars, size_t InIndexOf, EStringComparison Comparison = EStringComparison::CurrentCulture) const noexcept
	{
		if (Len == 0)
		{
			return (size_t)-1;
		}

		if (std::ranges::size(Chars) == 0)
		{
			return (size_t)-1;
		}

		const char_t* const MyBuf = this->GetRaw();
		const bool bLowerCase = Comparison == EStringComparison::CurrentCultureIgnoreCase;

		for (size_t i = InIndexOf; i < Len; ++i)
		{
			char_t Ch = SafeGet(MyBuf, Len, i, bLowerCase);

			bool bFound = false;
			if (bLowerCase)
			{
				bFound = Chars | Linq::Contains([&Ch](char_t Wc)
				{
					return Ch == (char_t)std::tolower(Wc);
				});
			}
			else
			{
				bFound = Chars | Linq::Contains(Ch);
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
		return Len == 0;
	}

	[[nodiscard]] inline constexpr bool IsValidIndex(size_t InIndex) const noexcept
	{
		return InIndex < Len;
	}

	[[nodiscard]] inline String Trim() const
	{
		return Trim(Char::k_WhiteSpaceChars);
	}

	template<std::ranges::input_range TCharArray>
		requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
	[[nodiscard]] inline String Trim(const TCharArray& Chars) const
	{
		return this->TrimAll((ETrimType)((int32)ETrimType::Head | (int32)ETrimType::Tail), Chars);
	}

	template<std::convertible_to<char_t>... TCharSequence>
	[[nodiscard]] inline String Trim(const TCharSequence&... Chars) const
	{
		return this->Trim(std::array{ (char_t)Chars... });
	}

	[[nodiscard]] inline String TrimStart() const
	{
		return TrimStart(Char::k_WhiteSpaceChars);
	}

	template<std::ranges::input_range TCharArray>
		requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
	[[nodiscard]] inline String TrimStart(const TCharArray& Chars) const
	{
		return this->TrimAll(ETrimType::Head, Chars);
	}

	template<std::convertible_to<char_t>... TCharSequence>
	[[nodiscard]] inline String TrimStart(const TCharSequence&... Chars) const
	{
		return TrimStart(std::array{ (char_t)Chars... });
	}

	[[nodiscard]] inline String TrimEnd() const
	{
		return TrimEnd(Char::k_WhiteSpaceChars);
	}

	template<std::ranges::input_range TCharArray>
		requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
	[[nodiscard]] inline String TrimEnd(const TCharArray& Chars) const
	{
		return this->TrimAll(ETrimType::Tail, Chars);
	}

	template<std::convertible_to<char_t>... TCharSequence>
	[[nodiscard]] inline String TrimEnd(const TCharSequence&... Chars) const
	{
		return TrimEnd(std::array{ (char_t)Chars... });
	}

	[[nodiscard]] String Substring(size_t StartIndex, size_t InLength = (size_t)-1) const
	{
		if (StartIndex >= Len)
		{
			return String();
		}

		InLength = std::min(InLength, Len - StartIndex);
		return String(std::wstring_view(this->GetRaw() + StartIndex, InLength));
	}

	[[nodiscard]] constexpr String SubstringView(size_t StartIndex, size_t InLength = -1) const noexcept
	{
		if (StartIndex >= Len)
		{
			return String::GetEmpty();
		}

		if (InLength > Len - StartIndex)
		{
			InLength = Len - StartIndex;
		}

		InLength = std::min(InLength, Len - StartIndex);
		return String::FromLiteral(std::wstring_view(this->GetRaw() + StartIndex, InLength));
	}

	[[nodiscard]] inline std::vector<String> Split(char_t Separator, EStringSplitOptions Options = EStringSplitOptions::None) const
	{
		std::array<char_t, 1> Seps{ Separator };
		return Split(Seps, Options);
	}

	template<std::ranges::input_range TCharArray>
		requires std::convertible_to<std::ranges::range_value_t<TCharArray>, char_t>
	[[nodiscard]] std::vector<String> Split(const TCharArray& Separators, EStringSplitOptions Options = EStringSplitOptions::None) const
	{
		std::vector<String> Results;

		const bool bRemoveEmpty = EnumHasFlags(Options, EStringSplitOptions::RemoveEmptyEntries);
		const bool bTrim = EnumHasFlags(Options, EStringSplitOptions::TrimEntries);

		for (size_t i = 0; i < Len;)
		{
			std::optional<String> View;
			size_t Seekp = IndexOfAny(Separators, i);

			if (Seekp == -1)
			{
				// Could not found separator.
				View = Substring(i);
				i = (size_t)-1;
			}
			else
			{
				size_t Length = Seekp - i;
				if (Length != 0 || !bRemoveEmpty)
				{
					View = Substring(i, Length);
				}
				i = Seekp + 1;
			}

			if (View)
			{
				if (bTrim)
				{
					View = View->Trim();
					if (bRemoveEmpty && View->Len == 0)
					{
						continue;
					}
				}

				Results.emplace_back(View.value());
			}
		}

		return Results;
	}

	[[nodiscard]] std::vector<String> Split(const String& Separator, EStringSplitOptions Options = EStringSplitOptions::None) const
	{
		std::vector<String> Results;

		const bool bRemoveEmpty = EnumHasFlags(Options, EStringSplitOptions::RemoveEmptyEntries);
		const bool bTrim = EnumHasFlags(Options, EStringSplitOptions::TrimEntries);

		for (size_t i = 0; i < Len;)
		{
			std::optional<String> View;
			size_t Seekp = IndexOf(Separator, i);

			if (Seekp == -1)
			{
				// Could not found separator.
				View = Substring(i);
				i = (size_t)-1;
			}
			else
			{
				size_t Length = Seekp - i;
				if (Length != 0 || !bRemoveEmpty)
				{
					View = Substring(i, Length);
				}
				i = Seekp + Separator.Len;
			}

			if (View)
			{
				if (bTrim)
				{
					View = Trim(View.value());
					if (bRemoveEmpty && View->Len == 0)
					{
						continue;
					}
				}

				Results.emplace_back(View.value());
			}
		}

		return Results;
	}

	template<class TOp>
	[[nodiscard]] String Transform(TOp&& Op) const requires
		std::convertible_to<std::invoke_result_t<TOp, char_t>, char_t>
	{
		decltype(Buf) Nbuf;
		auto& Ptr = Nbuf.emplace<1>(std::make_shared<char_t[]>(Len + 1));
		const char_t* const MyBuf = this->GetRaw();

		for (size_t i = 0; i < Len; ++i)
		{
			Ptr[i] = (char_t)Op(MyBuf[i]);
		}

		String S;
		S.Buf = std::move(Nbuf);
		S.Len = Len;
		return S;
	}

	[[nodiscard]] inline String ToLower() const
	{
		return Transform((int(*)(int))std::tolower);
	}

	[[nodiscard]] inline String ToUpper() const
	{
		return Transform((int(*)(int))std::toupper);
	}

	[[nodiscard]] String Replace(const String& OldValue, const String& NewValue, EStringComparison Comparison = EStringComparison::CurrentCulture) const
	{
		const char_t* const MyBuf = this->GetRaw();

		std::vector<std::wstring_view> Concats;

		for (size_t i = 0;;)
		{
			size_t InIndexOf = IndexOf(OldValue, i, Comparison);
			if (InIndexOf == -1)
			{
				size_t len = Len - i;
				Concats.emplace_back(std::wstring_view(MyBuf + i, len));
				break;
			}
			else
			{
				size_t len = InIndexOf - i;
				Concats.emplace_back(std::wstring_view(MyBuf + i, len));
				Concats.emplace_back(std::wstring_view(NewValue.GetRaw(), NewValue.Len));
				i = InIndexOf + OldValue.Len;
			}
		}

		return InternalConcat(Concats);
	}

	[[nodiscard]] String ReplaceAt(size_t InIndexOf, size_t InLength, String NewValue) const
	{
		if (!NewValue)
		{
			return *this;
		}

		if (InIndexOf >= Len)
		{
			return *this;
		}

		InLength = std::min(InLength, Len - InIndexOf);
		size_t Tail = InIndexOf + InLength;

		std::array<std::wstring_view, 3> Concats;
		Concats[0] = std::wstring_view(this->GetRaw(), InIndexOf);
		Concats[1] = std::wstring_view(NewValue.GetRaw(), NewValue.Len);
		Concats[2] = std::wstring_view(this->GetRaw() + Tail, Len - Tail);
		return InternalConcat(Concats);
	}

	[[nodiscard]] inline String Insert(size_t InIndexOf, String NewValue) const
	{
		return ReplaceAt(InIndexOf, 0, NewValue);
	}

	[[nodiscard]] inline String Insert(size_t InIndexOf, char_t InCh) const
	{
		char_t LocalBuf[2] = { InCh, 0 };
		return Insert(InIndexOf, String::FromLiteral(LocalBuf));
	}

	[[nodiscard]] inline String Remove(size_t InIndexOf, size_t InLength = -1) const
	{
		const size_t Rem = Len - InIndexOf;
		if (InLength > Rem || InLength == -1)
		{
			InLength = Rem;
		}

		std::array<std::wstring_view, 2> Concats;
		Concats[0] = std::wstring_view(this->GetRaw(), InIndexOf);
		size_t App = InIndexOf + InLength;
		Concats[1] = std::wstring_view(this->GetRaw() + App, Len - App);
		return InternalConcat(Concats);
	}

	[[nodiscard]] inline String Quotes(char_t Ch = '"') const
	{
		std::array<std::wstring_view, 3> Concats;
		Concats[0] = std::wstring_view(&Ch, 1);
		Concats[1] = std::wstring_view(this->GetRaw(), Len);
		Concats[2] = std::wstring_view(&Ch, 1);
		return InternalConcat(Concats);
	}

public:
	[[nodiscard]] static inline constexpr String FromLiteral(std::wstring_view InStr) noexcept
	{
		//return String(Buf_t(InStr.data()), InStr.length());
		return String();
	}

	[[nodiscard]] static String FromLiteral(std::string_view InStr);

public:
	[[nodiscard]] static String FromCodepage(std::string_view Str, int32 Codepage = 0);
	[[nodiscard]] std::string AsCodepage(int32 Codepage = 0) const;

public:
	template<class... TArgs>
	[[nodiscard]] static String Format(String InFormatStr, TArgs&&... InArgs)
	{
		return String(std::vformat(InFormatStr.wstring_view(), std::make_wformat_args(std::forward<TArgs>(InArgs)...)));
	}

	template<std::ranges::input_range TStringArray>
		requires std::convertible_to<std::ranges::range_value_t<TStringArray>, String>
	[[nodiscard]] static String Concat(const TStringArray& Strings)
	{
		std::vector<std::wstring_view> Buffers;

		if constexpr (std::ranges::sized_range<TStringArray>)
		{
			Buffers.reserve(std::ranges::size(Strings));
		}

		for (const String& Item : Strings)
		{
			Buffers.emplace_back(std::wstring_view(Item.GetRaw(), Item.Len));
		}

		return InternalConcat(Buffers);
	}

	template<std::convertible_to<String>... TStringSequence>
	[[nodiscard]] static inline String Concat(const TStringSequence&... Strings)
	{
		return Concat(std::array{ (String)Strings... });
	}

	template<std::ranges::input_range TStringArray>
		requires std::convertible_to<std::ranges::range_value_t<TStringArray>, String>
	[[nodiscard]] static String Join(const String& Separator, TStringArray&& Strings)
	{
		std::vector<String> Concats;

		if constexpr (std::ranges::sized_range<TStringArray>)
		{
			Concats.reserve(std::ranges::size(Strings));
		}

		for (const auto& Item : Strings)
		{
			if (Concats.size() > 0)
			{
				Concats.emplace_back(Separator);
			}

			Concats.emplace_back(Item);
		}

		return Concat(Concats);
	}

	template<std::convertible_to<String>... TStringSequence>
	[[nodiscard]] static inline String Join(const String& Separator, const TStringSequence&... Strings)
	{
		return Join(Separator, std::array{ (String)Strings... });
	}

	[[nodiscard]] static inline constexpr String GetEmpty() noexcept
	{
		// no allocate
		return String();
	}

private:
	template<class T>
	static constexpr bool EnumHasFlags(T flags, T value)
	{
		return ((int32)flags & (int32)value) != 0;
	}
};

template<>
struct std::formatter<String, wchar_t> : public std::formatter<std::wstring_view, wchar_t>
{
	template<class TFormatContext>
	auto format(const String& InStr, TFormatContext& Context) const
	{
		return std::formatter<std::wstring_view, wchar_t>::format((std::wstring_view)InStr, Context);
	}
};

template<>
struct std::formatter<String, char> : public std::formatter<std::string, char>
{
	template<class TFormatContext>
	auto format(const String& InStr, TFormatContext& Context) const
	{
		return std::formatter<std::string_view, char>::format((std::string)InStr, Context);
	}
};

template<class T, class TChar> requires requires { { std::declval<T>().ToString() } -> std::convertible_to<String>; }
struct std::formatter<T, TChar> : public std::formatter<String, TChar>
{
	template<class U, class TFormatContext>
	auto format(U&& Obj, TFormatContext& Context) const
	{
		return std::formatter<String, TChar>::format(std::forward<U>(Obj).ToString(), Context);
	}
};

// Declared in CharType.h
constexpr FORCEINLINE String Char::ToStringView(const char_t& Ch) noexcept
{
	return String::FromLiteral(std::wstring_view(&Ch, 1));
}

FORCEINLINE String Char::ToString(char_t Ch)
{
	return ToStringView(Ch).Clone();
}