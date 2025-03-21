// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <concepts>
#include "GC/BasePtr.h"

namespace Ayla
{
	class Object;
}

namespace Ayla::GC
{
	template<std::derived_from<Object> T>
	class RPtr : public BasePtr
	{
	private:
		T* m_Ptr = nullptr;

	public:
		inline RPtr(T* ptr = nullptr) noexcept
		{
			m_Object = ptr;
			m_Ptr = ptr;
		}

		inline ~RPtr() noexcept
		{
		}

		inline T* Get() const noexcept { return m_Ptr; }
	};
}