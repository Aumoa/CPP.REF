// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "GC/RPtr.h"
#include "GC/PPtr.h"
#include "GC/GC.h"
#include "Object.h"

namespace Ayla
{
	template<class T>
	template<class U>
	inline PPtr<T>::PPtr(const RPtr<U>& rhs) noexcept requires std::derived_from<U, T>
	{
		m_Object = rhs.m_Object;
		m_Ptr = rhs.m_Ptr;
	}

	template<class T>
	template<class U>
	inline PPtr<T>::PPtr(RPtr<U>&& rhs) noexcept requires std::derived_from<U, T>
	{
		std::swap(m_Object = nullptr, rhs.m_Object);
		m_Ptr = rhs.m_Ptr;
		rhs.m_Ptr = nullptr;
		// Since the target is an RPtr, the root reference of the target must be removed.
		if (m_Ptr != nullptr)
		{
			GC::Release(m_Ptr);
		}
	}

	template<class T>
	inline RPtr<T>::RPtr(T* ptr) noexcept requires std::derived_from<T, Object>
	{
		m_Object = ptr;
		m_Ptr = ptr;
		if (m_Ptr != nullptr)
		{
			GC::AddRef(m_Ptr);
		}
	}

	template<class T>
	template<class U>
	inline RPtr<T>::RPtr(const RPtr<U>& rhs) noexcept requires std::derived_from<U, T>
	{
		m_Object = rhs.m_Object;
		m_Ptr = rhs.m_Ptr;
		if (m_Ptr != nullptr)
		{
			GC::AddRef(m_Ptr);
		}
	}

	template<class T>
	template<class U>
	inline RPtr<T>::RPtr(const PPtr<U>& rhs) noexcept requires std::derived_from<U, T>
	{
		m_Object = rhs.m_Object;
		m_Ptr = rhs.m_Ptr;
		if (m_Ptr != nullptr)
		{
			GC::AddRef(m_Ptr);
		}
	}

	template<class T>
	template<class U>
	inline RPtr<T>::RPtr(PPtr<U>&& rhs) noexcept requires std::derived_from<U, T>
	{
		std::swap(m_Object = nullptr, rhs.m_Object);
		m_Ptr = rhs.m_Ptr;
		if (m_Ptr != nullptr)
		{
			GC::AddRef(m_Ptr);
		}
	}

	template<class T>
	inline RPtr<T>::~RPtr() noexcept
	{
		if (m_Ptr != nullptr)
		{
			GC::Release(m_Ptr);
			m_Ptr = nullptr;
			m_Object = nullptr;
		}
	}
}