// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <algorithm>
#include "GC/BasePtr.h"

namespace Ayla
{
	class Object;

	template<class T>
	class PPtr;

	template<class T>
	class RPtr : public BasePtr
	{
		template<class U>
		friend class PPtr;
		template<class U>
		friend class RPtr;

	private:
		T* m_Ptr = nullptr;

	public:
		inline RPtr() noexcept
		{
			m_Object = nullptr;
			m_Ptr = nullptr;
		}

		inline explicit RPtr(T* ptr) noexcept requires std::derived_from<T, Object>;
		template<class U>
		inline RPtr(const RPtr<U>& rhs) noexcept requires std::derived_from<U, T>;

		template<class U>
		inline RPtr(RPtr<U>&& rhs) noexcept requires std::derived_from<U, T>
		{
			std::swap(m_Object = nullptr, rhs.m_Object);
			m_Ptr = rhs.m_Ptr;
			rhs.m_Ptr = nullptr;
		}

		template<class U>
		inline RPtr(const PPtr<U>& rhs) noexcept requires std::derived_from<U, T>;
		template<class U>
		inline RPtr(PPtr<U>&& rhs) noexcept requires std::derived_from<U, T>;

		inline ~RPtr() noexcept;

		inline T* Get() const noexcept
		{
			return m_Ptr;
		}

		inline RPtr<T>& operator =(const RPtr<T>& rhs) noexcept;
		inline RPtr<T>& operator =(RPtr<T>&& rhs) noexcept;
		inline RPtr<T>& operator =(std::nullptr_t) noexcept;

		inline T* operator ->() const noexcept
		{
			return m_Ptr;
		}
	};
}