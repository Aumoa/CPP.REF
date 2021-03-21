// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Core/Object.h"

#include <vector>
#include <string>
#include <optional>
#include <span>
#include "CoreConcepts.h"
#include "TRefPtr.h"
#include "IStringFormattable.h"
#include "IEnumerable.h"

template<class T>
concept TIsStringConvertible = requires(const T& arg)
{
	{ arg.ToString() };
};

template<class T>
constexpr bool VIsStringConvertible = false;
template<TIsStringConvertible T>
constexpr bool VIsStringConvertible<T> = true;

template<class T>
constexpr bool VIsFormattableStringConvertible = false;
template<TIsFormattableStringConvertible T>
constexpr bool VIsFormattableStringConvertible<T> = true;

template<class T>
concept TIsOnlyStringConvertible = VIsStringConvertible<T> && !VIsFormattableStringConvertible<T>;

template<class TItem>
using StringIterator = TItem*;

class CORE_API String : virtual public Object, virtual public IConstEnumerable<StringIterator, wchar_t>
{
public:
	using Super = Object;
	using This = String;
	using Iterator = StringIterator<wchar_t>;
	using ConstIterator = StringIterator<const wchar_t>;

private:
	static wchar_t EmptyBuffer[1];

	const wchar_t* text_buffer;
	size_t len;
	bool bDynamicBuffer : 1;
	mutable std::optional<size_t> hash_cache;

public:
	String();
	~String() override;

	TRefPtr<String> ToString() const override;
	bool Equals(TRefPtr<Object> right) const override;
	size_t GetHashCode() const override;

	String(const char* text);
	String(const wchar_t* text);
	String(const char* text, size_t len);
	String(const wchar_t* text, size_t len);
	String(const std::string& text);
	String(const std::wstring& text);
	String(std::string_view text);
	String(std::wstring_view text);
	template<TIsIterator<char> TIterator>
	String(TIterator begin, TIterator end);
	template<TIsIterator<wchar_t> TIterator>
	String(TIterator begin, TIterator end);

	virtual ConstIterator cbegin() const override;
	virtual ConstIterator cend() const override;

	TRefPtr<String> Substring(size_t startIndex, std::optional<size_t> length = std::nullopt) const;
	std::optional<size_t> IndexOf(TRefPtr<String> value, size_t startIndex = 0, bool bIgnoreCase = false) const;
	std::optional<size_t> IndexOf(wchar_t value, size_t startIndex = 0, bool bIgnoreCase = false) const;
	std::optional<size_t> IndexOfAny(const wchar_t* value_sequence, size_t length, size_t startIndex = 0, bool bIgnoreCase = false) const;

	vs_property_get(const wchar_t*, C_Str);
	vs_property_get(size_t, Length);

	bool operator < (const TRefPtr<String>& right) const;
	bool operator ==(const TRefPtr<String>& right) const;
	bool operator !=(const TRefPtr<String>& right) const;
	wchar_t operator [](size_t index) const;

	template<class... TArgs>
	static TRefPtr<String> Format(TRefPtr<String> format, TArgs&&... args);
	static TRefPtr<String> Format(TRefPtr<String> format);
	template<class... TArgs>
	static TRefPtr<String> Join(TRefPtr<String> separator, TRefPtr<String> arg1, TRefPtr<String> arg2, TArgs&&... values);
	template<class T, size_t N> requires TIsStringConvertible<T> || TIsStringConstructible<T>
	static TRefPtr<String> Join(TRefPtr<String> separator, const T(&values)[N]);
	template<THasStdEnumerable TContainer> requires TIsStringConvertible<decltype(*std::begin(TContainer()))>
	static TRefPtr<String> Join(TRefPtr<String> separator, const TContainer& values);
	template<THasStdEnumerable TContainer> requires TIsStringConstructible<decltype(*std::begin(TContainer()))>
	static TRefPtr<String> Join(TRefPtr<String> separator, const TContainer& values);
	static TRefPtr<String> Join(TRefPtr<String> separator, const std::span<TRefPtr<String>>& values);
	template<class... TArgs>
	static TRefPtr<String> Concat(TRefPtr<String> arg1, TRefPtr<String> arg2, TArgs&&... values);
	template<class T, size_t N> requires TIsStringConvertible<T> || TIsStringConstructible<T>
	static TRefPtr<String> Concat(const T(&values)[N]);
	template<THasStdEnumerable TContainer> requires TIsStringConvertible<decltype(*std::begin(TContainer()))>
	static TRefPtr<String> Concat(const TContainer& values);
	template<THasStdEnumerable TContainer> requires TIsStringConstructible<decltype(*std::begin(TContainer()))>
	static TRefPtr<String> Concat(const TContainer& values);
	static TRefPtr<String> Concat(const std::span<TRefPtr<String>>& values);

	static const TRefPtr<String> Empty;

private:
	static TRefPtr<String> FormatHelper(TRefPtr<String> format, const std::span<TRefPtr<Object>>& unpackedArgs);
	wchar_t* MultiByteToWideChar(const char* multibyte, size_t* len);
	wchar_t* CopyAllocate(const wchar_t* text, size_t len);

	template<TIsChar TChar>
	static TChar* AllocCharBuffer(size_t len);
	template<TIsChar TChar>
	static void Free(TChar* charBuffer);
	static void* AllocInternal(size_t sizeInBytes);
	static void FreeInternal(void* internalPointer);

	void Assign(const char* text, size_t len);
	void Assign(const wchar_t* text, size_t len);

	template<TIsChar TChar>
	inline static size_t Strlen(const TChar* text);
	template<TIsChar TChar>
	inline static bool Strcmp(const TChar* left, const TChar* right);
	static size_t SizeAsBoundary(size_t len);

	template<TIsRefCore T>
	static TRefPtr<Object> GetString(TRefPtr<T> packedArg);
	template<TIsPrimitive T>
	static TRefPtr<Object> GetString(T packedArg);
	template<TIsChar T>
	static TRefPtr<Object> GetString(const T* packedArg);
	template<TIsOnlyStringConvertible T>
	static TRefPtr<Object> GetString(const T& packedArg);
	template<TIsFormattableStringConvertible T>
	static TRefPtr<Object> GetString(const T& packedArg);
	template<TIsBaseOf<Object> T>
	static TRefPtr<Object> GetString(const T* packedArg);

	template<class T, class... TArgs>
	static void FormatUnpack(std::vector<TRefPtr<Object>>& container, size_t index, T&& arg, TArgs&&... args);
};

#include "String.inl"