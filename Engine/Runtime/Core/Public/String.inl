// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include <sstream>
#include <iomanip>

namespace SC::Runtime::Core
{	
	template<TIsIterator<char> TIterator>
	inline String::String(TIterator begin, TIterator end)
	{
		size_t length = end - begin;
		auto buffer = HeapStorage(AllocCharBuffer<char>(length + 1));
		auto seek = buffer.ptr;
		for (auto it = begin; it != end; ++it)
		{
			*seek++ = *it;
		}

		Assign(buffer.ptr, length);
	}

	template<TIsIterator<wchar_t> TIterator>
	inline String::String(TIterator begin, TIterator end)
	{
		size_t length = end - begin;
		auto buffer = HeapStorage(AllocCharBuffer<wchar_t>(length + 1));
		auto seek = buffer.ptr;
		for (auto it = begin; it != end; ++it)
		{
			*seek++ = *it;
		}

		text_buffer = buffer.ptr;
		buffer.ptr = nullptr;
		len = length;
		bDynamicBuffer = true;
	}

	template<class... TArgs>
	inline TRefPtr<String> String::Format(TRefPtr<String> format, TArgs... args)
	{
		std::vector<TRefPtr<Object>> unpacked(sizeof...(TArgs));
		if constexpr (sizeof...(TArgs) != 0)
		{
			FormatUnpack(unpacked, 0, args...);
		}
		return FormatHelper(format, unpacked);
	}

	template<TIsChar TChar>
	inline TChar* String::AllocCharBuffer(size_t len)
	{
		len = SizeAsBoundary(len + 1);
		TChar* buffer = (TChar*)AllocInternal(sizeof(TChar) * len);
		memset(buffer, 0, (len) * sizeof(TChar));
		return buffer;
	}

	template<TIsChar TChar>
	inline void String::Free(TChar* charBuffer)
	{
		FreeInternal(charBuffer);
	}

	template<TIsChar TChar>
	inline size_t String::Strlen(const TChar* text)
	{
		size_t length = 0;
		for (auto item = text; *item != 0; ++item)
		{
			length += 1;
		}
		return length;
	}

	template<TIsChar TChar>
	inline bool String::Strcmp(const TChar* left, const TChar* right)
	{
		while (true)
		{
			if (*left == 0 || *right == 0)
			{
				return *left == *right;
			}

			else if (*left != *right)
			{
				return false;
			}

			++left;
			++right;
		}

		return false;
	}

	template<TIsRefCore T>
	TRefPtr<Object> String::GetString(TRefPtr<T> packedArg)
	{
		return packedArg;
	}

	template<TIsPrimitive T>
	TRefPtr<Object> String::GetString(T packedArg)
	{
		class PrimitiveString : virtual public Object, virtual public IStringFormattable
		{
			T value;

		public:
			PrimitiveString(T value) : value(value)
			{

			}

			~PrimitiveString() override
			{

			}

			TRefPtr<String> ToString() const override
			{
				std::wstringstream woss;
				woss << value;
				return woss.str().c_str();
			}

			virtual TRefPtr<String> ToString(TRefPtr<String> formatText) const
			{
				std::wstringstream woss;
				if (formatText[0] == L'x')
				{
					woss << std::hex;
				}
				else if (formatText[0] == L'X')
				{
					woss << std::uppercase << std::hex;
				}

				if (formatText->Length >= 2)
				{
					size_t pad = 0;
					std::wistringstream wiss;
					wiss.str(formatText->C_Str + 1);
					wiss >> pad;

					if (pad != 0)
					{
						woss << std::setfill(L'0') << std::setw(pad);
					}
				}

				woss << value;
				return woss.str().c_str();
			}
		};

		return NewObject<PrimitiveString>(packedArg);
	}

	template<TIsChar T>
	TRefPtr<Object> String::GetString(const T* packedArg)
	{
		return packedArg;
	}

	template<TIsOnlyStringConvertible T>
	TRefPtr<Object> String::GetString(const T& packedArg)
	{
		return packedArg.ToString();
	}

	template<TIsFormattableStringConvertible T>
	TRefPtr<Object> String::GetString(const T& packedArg)
	{
		class FormattableString : virtual public Object, virtual public IStringFormattable
		{
			const T& value;

		public:
			FormattableString(const T& value) : value(value)
			{

			}

			~FormattableString() override
			{

			}

			TRefPtr<String> ToString() const override
			{
				return value.ToString();
			}

			virtual TRefPtr<String> ToString(TRefPtr<String> formatText) const
			{
				return value.ToString(formatText);
			}
		};

		return NewObject<FormattableString>(packedArg);
	}

	template<TIsBaseOf<Object> T>
	TRefPtr<Object> String::GetString(const T* packedArg)
	{
		return packedArg;
	}

	template<class T, class... TArgs>
	static void String::FormatUnpack(std::vector<TRefPtr<Object>>& container, size_t index, T arg, TArgs... args)
	{
		container[index++] = GetString(arg);

		if constexpr (sizeof...(TArgs) != 0)
		{
			FormatUnpack(container, index, args...);
		}
	}

	template<TIsNotPointer T> template<TIsStringConstructible TStringConstructibleArg> requires TIsAssignable<String, T>
	inline TRefPtr<T>::TRefPtr(TStringConstructibleArg arg) : This(new String(arg))
	{
		ptr->bLockCollecting = false;
	}

	template<TIsNotPointer T> template<TIsChar TChar> requires TIsSame<T, String>
	inline bool TRefPtr<T>::operator ==(const TChar* text) const
	{
		return ptr->operator ==(TRefPtr<String>(text));
	}

	template<TIsNotPointer T> template<TIsChar TChar> requires TIsSame<T, String>
	inline bool TRefPtr<T>::operator !=(const TChar* text) const
	{
		return ptr->operator !=(TRefPtr<String>(text));
	}
}