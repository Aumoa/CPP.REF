// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if PLATFORM_WINDOWS
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;
#else

namespace Ayla
{
	template<class T>
	class ComPtr
	{
		T* m_Ptr = nullptr;

	public:
		ComPtr() = default;
		
		ComPtr(const ComPtr& other) : m_Ptr(other.m_Ptr)
		{
			if (m_Ptr)
			{
				m_Ptr->AddRef();
			}
		}
		
		~ComPtr()
		{
			Reset();
		}
		
		void Reset()
		{
			if (m_Ptr)
			{
				m_Ptr->Release();
				m_Ptr = nullptr;
			}
		}
		
		T* Get() const noexcept
		{
			return m_Ptr;
		}

		T** ReleaseAndGetAddressOf() noexcept
		{
			Reset();
			return &m_Ptr;
		}

		T** operator &() noexcept
		{
			return ReleaseAndGetAddressOf();
		}
		
		T* operator ->() const noexcept
		{
			return m_Ptr;
		}
		
		ComPtr& operator=(const ComPtr& other)
		{
			if (this != &other)
			{
				Reset();
				m_Ptr = other.m_Ptr;
				if (m_Ptr)
				{
					m_Ptr->AddRef();
				}
			}
			return *this;
		}
	};
}

#endif