// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <algorithm>
#include "GC/BasePtr.h"

namespace Ayla
{
	class Object;

	template<class T>
	class RPtr;

	template<class T>
	class PPtr : public BasePtr
	{
		template<class U>
		friend class RPtr;
		template<class U>
		friend class PPtr;

	private:
		T* m_Ptr = nullptr;

	public:
		inline PPtr() noexcept
		{
			m_Object = nullptr;
			m_Ptr = nullptr;
		}

		inline explicit PPtr(T* ptr) noexcept requires std::derived_from<T, Object>;
		template<class U>
		inline PPtr(const PPtr<U>& rhs) noexcept requires std::derived_from<U, T>;
		template<class U>
		inline PPtr(PPtr<U>&& rhs) noexcept requires std::derived_from<U, T>;
		template<class U>
		inline PPtr(const RPtr<U>& rhs) noexcept requires std::derived_from<U, T>;
		template<class U>
		inline PPtr(RPtr<U>&& rhs) noexcept requires std::derived_from<U, T>;

		inline ~PPtr() noexcept
		{
			m_Object = nullptr;
			m_Ptr = nullptr;
		}

		inline T* Get() const noexcept
		{
			return m_Ptr;
		}

		inline PPtr<T>& operator =(const PPtr<T>& rhs) noexcept;
		inline PPtr<T>& operator =(PPtr<T>&& rhs) noexcept;
		inline PPtr<T>& operator =(std::nullptr_t) noexcept;

		inline T* operator ->() const noexcept
		{
			return m_Ptr;
		}
	};
}
