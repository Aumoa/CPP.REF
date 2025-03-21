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
	class PPtr : public BasePtr
	{
	private:
		T* m_Ptr = nullptr;

	public:
		inline PPtr(T* ptr = nullptr) noexcept
		{
			m_Object = ptr;
			m_Ptr = ptr;
		}

		inline ~PPtr() noexcept
		{
		}

		inline T* Get() const noexcept { return m_Ptr; }
	};
}