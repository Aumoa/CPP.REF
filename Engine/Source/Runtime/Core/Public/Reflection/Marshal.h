// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "StaticClass.h"
#include "GC/PPtr.h"
#include "GC/RPtr.h"

namespace Ayla
{
	class Marshal : public StaticClass
	{
	private:
		template<class T>
		static T MarshalToBinding(T&& value, short)
		{
			return std::forward<T>(value);
		}

		template<class T>
		static void* MarshalToBinding(T&& ptr, int) requires std::derived_from<std::remove_reference_t<T>, BasePtr>
		{
			return (Object*)ptr.Get();
		}

	public:
		template<class T>
		static auto MarshalToBinding(T&& value)
		{
			return MarshalToBinding(std::forward<T>(value), 0);
		}

	private:
		template<class TBinding, class TNative>
		static TNative MarshalToNative(TBinding&& value, short)
		{
			return std::forward<TBinding>(value);
		}

		template<class TBinding, class TNative>
		static TNative MarshalToNative(TBinding&& value, int) requires
			std::derived_from<TNative, BasePtr> &&
			std::same_as<std::remove_reference_t<TBinding>, void*>
		{
			auto ptr = (Object*)value;
			return TNative(ptr);
		}

	public:
		template<class TBinding, class TNative>
		static auto MarshalToNative(TBinding&& value)
		{
			return MarshalToNative<TBinding, TNative>(std::forward<TBinding>(value), 0);
		}
	};
}