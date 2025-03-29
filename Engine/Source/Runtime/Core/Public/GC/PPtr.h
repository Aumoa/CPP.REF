// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include <algorithm>
#include "GC/BasePtr.h"
#include "GC/CommonPtrDeclaration.h"

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

		inline PPtr(T* ptr) noexcept requires std::derived_from<T, Object>
		{
			m_Object = ptr;
			m_Ptr = ptr;
		}

		template<class U>
		inline PPtr(const PPtr<U>& rhs) noexcept requires std::derived_from<U, T>
		{
			m_Object = rhs.m_Object;
			m_Ptr = rhs.m_Ptr;
		}

		template<class U>
		inline PPtr(PPtr<U>&& rhs) noexcept requires std::derived_from<U, T>
		{
			std::swap(m_Object = nullptr, rhs.m_Object);
			m_Ptr = rhs.m_Ptr;
			rhs.m_Ptr = nullptr;
		}

		template<class U>
		inline PPtr(const RPtr<U>& rhs) noexcept requires std::derived_from<U, T>;
		template<class U>
		inline PPtr(RPtr<U>&& rhs) noexcept requires std::derived_from<U, T>;

		inline ~PPtr() noexcept
		{
			m_Object = nullptr;
			m_Ptr = nullptr;
		}

#define AYLA__COMMON_PTR_CLASS_NAME PPtr
		AYLA__COMMON_PTR_DECLARATION;
#undef AYLA__COMMON_PTR_CLASS_NAME
	};
}