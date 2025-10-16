// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "InvalidCastException.h"
#include <algorithm>
#include <compare>

namespace Ayla
{
	template<class T>
	class PPtr
	{
		T* m_Ptr;

	public:
		constexpr PPtr() noexcept
			: m_Ptr(nullptr)
		{
		}

		inline explicit PPtr(T* ptr) noexcept
			: m_Ptr(ptr)
		{
		}

		inline PPtr(const PPtr& other) noexcept
			: m_Ptr(other.m_Ptr)
		{
			AddRef();
		}

		inline PPtr(PPtr&& other) noexcept
			: m_Ptr(other.m_Ptr)
		{
			other.m_Ptr = nullptr;
		}

		template<std::derived_from<T> U>
		inline PPtr(const PPtr<U>& other) noexcept
			: m_Ptr(other.m_Ptr)
		{
			AddRef();
		}

		template<std::derived_from<T> U>
		inline PPtr(PPtr<U>&& other) noexcept
			: m_Ptr(other.m_Ptr)
		{
			AddRef();
		}

		template<class U> requires std::derived_from<U, T>
		inline PPtr(const PPtr<U>& other)
			: m_Ptr(dynamic_cast<T*>(other.m_Ptr))
		{
			if (other.m_Ptr && m_Ptr == nullptr)
			{
				ThrowInvalidCast<U, T>();
			}

			AddRef();
		}

		template<class U> requires std::derived_from<U, T>
		inline PPtr(PPtr<U>&& other)
			: m_Ptr(dynamic_cast<T*>(other.m_Ptr))
		{
			if (other.m_Ptr && m_Ptr == nullptr)
			{
				ThrowInvalidCast<U, T>();
			}

			other.m_Ptr = nullptr;
		}

		inline void AddRef()
		{
			if (m_Ptr)
			{
				m_Ptr->AddRef();
			}
		}

		inline void Release()
		{
			T* oldPtr = nullptr;
			std::swap(oldPtr, m_Ptr);

			if (oldPtr)
			{
				oldPtr->ReleaseRef();
			}
		}

		template<class S>
		constexpr auto Get(this S&& self) noexcept { return const_cast<T*>(self.m_Ptr); }

		constexpr operator bool() const noexcept { return m_Ptr; }
		constexpr auto operator <=>(const PPtr& other) const noexcept { return m_Ptr <=> other.m_Ptr; }

		template<class S>
		constexpr auto operator ->(this S&& self) noexcept { return const_cast<T*>(self.m_Ptr); }

	private:
		template<class TFrom, class TTo>
		static [[noreturn]] void ThrowInvalidCast()
		{
			throw InvalidCastException(String::Format(TEXT("Cannot convert {} to {}"), String::FromLiteral(typeid(TFrom).name()), String::FromLiteral(typeid(TTo).name())), {});
		}
	};
}