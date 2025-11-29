// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#if !PLATFORM_WINDOWS

namespace Ayla
{
	template<class T>
	class ComPtr
	{
		T* m_Ptr = nullptr;

	public:
		void Reset()
		{
			if (m_Ptr)
			{
				m_Ptr->Release();
				m_Ptr = nullptr;
			}
		}

		T** ReleaseAndGetAddressOf() noexcept
		{
		}

		T** operator &() noexcept
		{
			return ReleaseAndGetAddressOf();
		}
	};
}

#endif