// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "Object.h"

#include "CoreConcepts.h"
#include "TRefPtr.h"
#include <vector>
#include "IStringFormattable.h"

namespace SC::Runtime::Core
{
	template<class T>
	concept TIsStringConvertible = requires(const T& arg)
	{
		{ arg.ToString() };
	};

	template<class T>
	concept TIsOnlyStringConvertible = requires
	{
		TIsStringConvertible<T>;
		!TIsFormattableStringConvertible<T>;
	};

	class CORE_API String : virtual public Object
	{
	public:
		static wchar_t EmptyBuffer[1];

		using Super = Object;
		using This = String;

	private:
		wchar_t* text_buffer;
		size_t len;
		bool bDynamicBuffer : 1;

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

		vs_property_get(const wchar_t*, C_Str);
		const wchar_t* C_Str_get() const;
		vs_property_get(size_t, Length);
		size_t Length_get() const;

		bool operator ==(const TRefPtr<String>& right) const;
		bool operator !=(const TRefPtr<String>& right) const;
		wchar_t operator [](size_t index) const;

		template<class... TArgs>
		static TRefPtr<String> Format(TRefPtr<String> format, TArgs... args);
		static TRefPtr<String> Format(TRefPtr<String> format);

	private:
		static TRefPtr<String> FormatHelper(TRefPtr<String> format, std::vector<TRefPtr<Object>>& unpackedArgs);
		wchar_t* MultiByteToWideChar(const char* multibyte, size_t* len);
		wchar_t* CopyAllocate(const wchar_t* text, size_t len);

		template<class TChar> requires TIsChar<TChar>
		inline static size_t Strlen(const TChar* text);
		template<class TChar> requires TIsChar<TChar>
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

		template<class T, class... TArgs> requires requires(const T& arg) { { String::GetString(arg) }; }
		static void FormatUnpack(std::vector<TRefPtr<Object>>& container, size_t index, T arg, TArgs... args);
	};
}

#include "String.inl"