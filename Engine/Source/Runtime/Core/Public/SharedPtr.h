// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "String_.h"
#include "InvalidCastException.h"
#include <algorithm>
#include <compare>

namespace Ayla
{
	template<class T>
	class SharedPtr
	{
		template<class>
		friend class SharedPtr;

	private:
		T* m_Ptr;

	public:
		constexpr SharedPtr() noexcept
			: m_Ptr(nullptr)
		{
		}

		constexpr SharedPtr(std::nullptr_t) noexcept
			: m_Ptr(nullptr)
		{
		}

		inline explicit SharedPtr(T* ptr) noexcept
			: m_Ptr(ptr)
		{
		}

		inline SharedPtr(const SharedPtr& other) noexcept
			: m_Ptr(other.m_Ptr)
		{
			AddRef();
		}

		constexpr SharedPtr(SharedPtr&& other) noexcept
			: m_Ptr(other.m_Ptr)
		{
			other.m_Ptr = nullptr;
		}

		template<std::derived_from<T> U>
		inline SharedPtr(const SharedPtr<U>& other) noexcept
			: m_Ptr(other.m_Ptr)
		{
			AddRef();
		}

		template<std::derived_from<T> U>
		constexpr SharedPtr(SharedPtr<U>&& other) noexcept
			: m_Ptr(other.m_Ptr)
		{
			AddRef();
		}

		template<class U> requires std::derived_from<T, U>
		inline SharedPtr(const SharedPtr<U>& other)
			: m_Ptr(dynamic_cast<T*>(other.m_Ptr))
		{
			if (other.m_Ptr && m_Ptr == nullptr)
			{
				ThrowInvalidCast<U, T>();
			}

			AddRef();
		}

		template<class U> requires std::derived_from<T, U>
		inline SharedPtr(SharedPtr<U>&& other)
			: m_Ptr(dynamic_cast<T*>(other.m_Ptr))
		{
			if (other.m_Ptr && m_Ptr == nullptr)
			{
				ThrowInvalidCast<U, T>();
			}

			other.m_Ptr = nullptr;
		}

		inline ~SharedPtr() noexcept
		{
			Release();
		}

		inline void AddRef()
		{
			if (m_Ptr)
			{
				m_Ptr->AddRef();
			}
		}

		inline void Release() noexcept
		{
			T* oldPtr = nullptr;
			std::swap(oldPtr, m_Ptr);

			if (oldPtr)
			{
				oldPtr->ReleaseRef();
			}
		}

		template<std::derived_from<T> U>
		inline SharedPtr<U> As() const
		{
			auto* cast = dynamic_cast<U*>(m_Ptr);
			if (cast)
			{
				cast->AddRef();
				return SharedPtr<U>(cast);
			}
			else if (m_Ptr != nullptr)
			{
				ThrowInvalidCast<T, U>();
			}

			return nullptr;
		}

		template<class S>
		constexpr auto Get(this S&& self) noexcept { return const_cast<T*>(self.m_Ptr); }

		constexpr operator bool() const noexcept { return m_Ptr; }
		constexpr auto operator <=>(const SharedPtr& other) const noexcept { return m_Ptr <=> other.m_Ptr; }

		template<class S>
		constexpr auto operator ->(this S&& self) noexcept { return const_cast<T*>(self.m_Ptr); }

		inline SharedPtr& operator =(const SharedPtr& other) noexcept
		{
			Release();
			m_Ptr = other.m_Ptr;
			AddRef();
			return *this;
		}

		inline SharedPtr& operator =(SharedPtr&& other) noexcept
		{
			Release();
			std::swap(m_Ptr, other.m_Ptr);
			return *this;
		}

		template<std::derived_from<T> U>
		inline SharedPtr& operator =(const SharedPtr<U>& other) noexcept
		{
			Release();
			m_Ptr = other.m_Ptr;
			AddRef();
			return *this;
		}

		template<std::derived_from<T> U>
		inline SharedPtr& operator =(SharedPtr<U>&& other) noexcept
		{
			Release();
			m_Ptr = other.m_Ptr;
			other.m_Ptr = nullptr;
			return *this;
		}

	private:
		template<class TFrom, class TTo>
		[[noreturn]] static void ThrowInvalidCast()
		{
			throw InvalidCastException(String::Format(TEXT("Cannot convert {} to {}"), String::FromLiteral(typeid(TFrom).name()), String::FromLiteral(typeid(TTo).name())), {});
		}
	};
}