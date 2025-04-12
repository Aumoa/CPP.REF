// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "StaticClass.h"
#include "Platform/PlatformMacros.h"
#include "GC/RPtr.h"
#include "IntegralTypes.h"

namespace Ayla
{
	class Object;

	struct CORE_API Marshal : public StaticClass
	{
		template<class T = Object>
		static inline RPtr<T>& IntPtrToRPtr(ssize_t intptr)
		{
			return *reinterpret_cast<RPtr<T>*>(reinterpret_cast<RPtr<Object>*>(intptr));
		}

		static inline ssize_t RPtrToIntPtr(RPtr<Object>& ptr)
		{
			return reinterpret_cast<ssize_t>(&ptr);
		}
	};
}