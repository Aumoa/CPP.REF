// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "GC/RPtr.h"
#include "GC/PPtr.h"
#include "GC/GC.h"
#include "Object.h"
#include "Reflection/PPtrGather.h"

namespace Ayla
{
	template<class T>
	inline PPtr<T>::PPtr(T* ptr) noexcept requires std::derived_from<T, Object>
	{
		if (ptr != nullptr)
		{
			m_Object = ptr;
			m_Ptr = ptr;
		}
		else
		{
			m_Object = nullptr;
			m_Ptr = nullptr;
		}
	}

	template<class T>
	template<class U>
	inline PPtr<T>::PPtr(const PPtr<U>& rhs) noexcept requires std::derived_from<U, T>
	{
		if (rhs.m_Object != nullptr)
		{
			m_Object = rhs.m_Object;
			m_Ptr = rhs.m_Ptr;
		}
		else
		{
			m_Object = nullptr;
			m_Ptr = nullptr;
		}
	}

	template<class T>
	template<class U>
	inline PPtr<T>::PPtr(PPtr<U>&& rhs) noexcept requires std::derived_from<U, T>
	{
		if (rhs.m_Object != nullptr)
		{
			std::swap(m_Object = nullptr, rhs.m_Object);
			m_Ptr = rhs.m_Ptr;
			rhs.m_Ptr = nullptr;
		}
	}

	template<class T>
	template<class U>
	inline PPtr<T>::PPtr(const RPtr<U>& rhs) noexcept requires std::derived_from<U, T>
	{
		if (rhs.m_Object != nullptr)
		{
			m_Object = rhs.m_Object;
			m_Ptr = rhs.m_Ptr;
		}
	}

	template<class T>
	template<class U>
	inline PPtr<T>::PPtr(RPtr<U>&& rhs) noexcept requires std::derived_from<U, T>
	{
		if (rhs.m_Object != nullptr)
		{
			std::swap(m_Object = nullptr, rhs.m_Object);
			m_Ptr = rhs.m_Ptr;
			rhs.m_Ptr = nullptr;
			// Since the target is an RPtr, the root reference of the target must be removed.
			GC::Release(m_Ptr);
		}
	}

	template<class T>
	inline PPtr<T>& PPtr<T>::operator =(const PPtr<T>& rhs) noexcept
	{
		if (m_Object == rhs.m_Object)
		{
			return *this;
		}

		m_Object = rhs.m_Object;
		m_Ptr = rhs.m_Ptr;
		return *this;
	}

	template<class T>
	inline PPtr<T>& PPtr<T>::operator =(PPtr<T>&& rhs) noexcept
	{
		if (m_Object == nullptr && rhs.m_Object == nullptr)
		{
			return *this;
		}

		std::swap(m_Object = nullptr, rhs.m_Object);
		std::swap(m_Ptr = nullptr, rhs.m_Ptr);
		return *this;
	}

	template<class T>
	inline PPtr<T>& PPtr<T>::operator =(std::nullptr_t) noexcept
	{
		if (m_Object == nullptr)
		{
			return *this;
		}

		m_Object = nullptr;
		m_Ptr = nullptr;
		return *this;
	}

	template<class T>
	struct PPtrGather<PPtr<T>> : public PPtrGather<>
	{
	public:
		virtual void PullPPtrs(Object* pointer, size_t offset, std::vector<Object*>& output) override
		{
			auto pptr = reinterpret_cast<PPtr<T>*>(reinterpret_cast<byte*>(pointer) + offset);
			if (auto* ptr = pptr->Get(); ptr != nullptr)
			{
				output.emplace_back(ptr);
			}
		}
	};

	template<class T> requires std::ranges::sized_range<T> && std::derived_from<std::ranges::range_value_t<T>, BasePtr>
	struct PPtrGather<T> : public PPtrGather<>
	{
	public:
		virtual void PullPPtrs(Object* pointer, size_t offset, std::vector<Object*>& output) override
		{
			auto ptr = reinterpret_cast<std::vector<T>*>(reinterpret_cast<byte*>(pointer) + offset);
			for (auto& pptr : *ptr)
			{
				if (pptr.Get() != nullptr)
				{
					output.emplace_back(ptr.Get());
				}
			}
		}
	};

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
		if (rhs.m_Object != nullptr)
		{
			auto lock = GC::GetLock();
			m_Object = rhs.m_Object;
			m_Ptr = rhs.m_Ptr;
			GC::AddRef(m_Ptr);
			rhs.m_Object = nullptr;
			rhs.m_Ptr = nullptr;
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

	template<class T>
	inline RPtr<T>& RPtr<T>::operator =(const RPtr<T>& rhs) noexcept
	{
		if (m_Object != nullptr)
		{
			GC::Release(m_Object);
		}

		m_Object = rhs.m_Object;
		m_Ptr = rhs.m_Ptr;

		if (m_Object != nullptr)
		{
			GC::AddRef(m_Object);
		}

		return *this;
	}

	template<class T>
	inline RPtr<T>& RPtr<T>::operator =(RPtr<T>&& rhs) noexcept
	{
		if (m_Object != nullptr)
		{
			GC::Release(m_Object);
		}

		std::swap(m_Object = nullptr, rhs.m_Object);
		std::swap(m_Ptr = nullptr, rhs.m_Ptr);
		return *this;
	}

	template<class T>
	inline RPtr<T>& RPtr<T>::operator =(std::nullptr_t) noexcept
	{
		if (m_Object != nullptr)
		{
			GC::Release(m_Object);
		}

		m_Object = nullptr;
		m_Ptr = nullptr;
		return *this;
	}
}