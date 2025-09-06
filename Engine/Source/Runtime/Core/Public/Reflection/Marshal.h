// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "StaticClass.h"
#include "GC/PPtr.h"
#include "GC/RPtr.h"
#include "Reflection/ObjectReferenceWrapper.h"
#include "String_.h"
#include <ranges>

namespace Ayla
{
	class Marshal : public StaticClass
	{
	private:
		template<class T>
		static T ToBinding(T&& value, short)
		{
			return std::forward<T>(value);
		}

		template<class T>
		static ObjectReferenceWrapper ToBinding(T&& ptr, int) requires std::derived_from<std::remove_reference_t<T>, BasePtr>
		{
			return ptr->AsWrapper();
		}

	public:
		template<class T>
		static auto ToBinding(T&& value)
		{
			return ToBinding(std::forward<T>(value), 0);
		}

	private:
		template<class TNative, class TBinding>
		static TNative ToNative(TBinding&& value, short)
		{
			return std::forward<TBinding>(value);
		}

		template<class TNative, class TBinding>
		static TNative ToNative(TBinding&& value, int) requires
			std::derived_from<TNative, BasePtr> &&
			std::same_as<std::remove_reference_t<TBinding>, void*>
		{
			auto ptr = (Object*)value;
			return TNative(ptr);
		}

		template<class TNative, class TBinding>
		static TNative ToNative(TBinding&& value, int) requires std::same_as<TNative, String>
		{
			return std::wstring_view(value);
		}

		template<std::ranges::input_range TNative, class TBinding>
		static TNative ToNative(TBinding&& value, int) requires std::same_as<std::ranges::range_value_t<TNative>, String>
		{
			ssize_t* array = (ssize_t*)value;
			ssize_t count = array[0];
			std::vector<String> result;
			result.reserve(count);
			for (ssize_t i = 0; i < count; ++i)
			{
				result.emplace_back(String((const wchar_t*)array[i + 1]));
			}
			return result;
		}

	public:
		template<class TNative, class TBinding>
		static auto ToNative(TBinding&& value)
		{
			return ToNative<TNative>(std::forward<TBinding>(value), 0);
		}
	};
}