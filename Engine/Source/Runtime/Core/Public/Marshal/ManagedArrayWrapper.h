// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"
#include "ObjectReferenceWrapper.h"
#include "SharedPtr.h"

namespace Ayla
{
	struct ManagedArrayWrapper
	{
		const void* Data;
		int32 Length;

		template<class T>
		inline std::vector<T> AsArray() const
		{
			auto* ptr = reinterpret_cast<const T*>(Data);
			return std::vector<T>{ ptr, ptr + Length };
		}

		template<class T>
		inline std::vector<SharedPtr<T>> AsObjectArray() const
		{
			auto* ptr = reinterpret_cast<ManagedObjectReferenceWrapper*>(const_cast<void*>(Data));
			std::vector<SharedPtr<T>> output;
			output.reserve(Length);
			for (int32 i = 0; i < Length; ++i)
			{
				output.emplace_back(ptr[i].AsNative<T>());
			}
			return output;
		}

		inline std::vector<String> AsStringArray() const
		{
			auto* ptr = reinterpret_cast<const byte*>(Data);
			size_t seekpos = 0;

			int32 arrayLength = *(int32*)(ptr + seekpos);
			seekpos += sizeof(int32);

			std::vector<String> output;
			output.reserve(arrayLength);

			for (int32 i = 0; i < arrayLength; ++i)
			{
				int32 strLength = *(int32*)(ptr + seekpos);
				seekpos += sizeof(int32);
				String str{ (const char_t*)(ptr + seekpos), (size_t)strLength };
				seekpos += sizeof(char_t) * strLength;
				output.emplace_back(std::move(str));
			}

			return output;
		}

		static ManagedArrayWrapper FromArray(const std::vector<int32>& arr)
		{
			return ManagedArrayWrapper
			{
				.Data = arr.data(),
				.Length = (int32)arr.size()
			};
		}

		struct ObjectArrayBinder
		{
			const void* Data;
			int32 Length;

			inline operator ManagedArrayWrapper() const
			{
				return ManagedArrayWrapper
				{
					.Data = Data,
					.Length = Length
				};
			}
		};

		template<class T>
		static ObjectArrayBinder FromObjectArray(const std::vector<SharedPtr<T>>& array)
		{
			auto output = new NativeObjectReferenceWrapper[array.size()];
			for (size_t i = 0; i < array.size(); ++i)
			{
				output[i] = NativeObjectReferenceWrapper::FromObject(array[i]);
			}

			return ObjectArrayBinder
			{
				.Data = output,
				.Length = (int32)array.size()
			};
		}

		struct StringArrayBinder
		{
			const void* Data;
			int32 Length;

			inline operator ManagedArrayWrapper() const
			{
				return ManagedArrayWrapper
				{
					.Data = Data,
					.Length = Length
				};
			}
		};

		static StringArrayBinder FromStringArray(const std::vector<String>& array)
		{
			size_t length = sizeof(int32);
			for (size_t i = 0; i < array.size(); ++i)
			{
				length += sizeof(int32);
				length += sizeof(char_t) * array[i].length();
			}

			auto output = new byte[length];
			size_t seekpos = 0;

			*(int32*)(output + seekpos) = (int32)array.size();
			seekpos += sizeof(int32);

			for (size_t i = 0; i < array.size(); ++i)
			{
				*(int32*)(output + seekpos) = (int32)array[i].length();
				seekpos += sizeof(int32);
				size_t copySize = sizeof(char_t) * array[i].length();
				memcpy(output + seekpos, array[i].c_str(), copySize);
				seekpos += copySize;
			}

			return StringArrayBinder
			{
				.Data = output,
				.Length = (int32)length
			};
		}
	};
}
