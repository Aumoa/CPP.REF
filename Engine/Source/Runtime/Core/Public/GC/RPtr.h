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

		inline RPtr(T* ptr) noexcept requires std::derived_from<T, Object>;
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

#define AYLA__COMMON_PTR_CLASS_NAME RPtr
		AYLA__COMMON_PTR_DECLARATION;
#undef AYLA__COMMON_PTR_CLASS_NAME
	};
}