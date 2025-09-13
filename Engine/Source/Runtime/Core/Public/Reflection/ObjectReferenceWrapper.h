// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "IntegralTypes.h"
#include <memory>

namespace Ayla
{
	class Object;

	struct CORE_API ObjectReferenceWrapper
	{
		ssize_t Ptr;
		ssize_t Handle;

		template<class T>
		inline std::shared_ptr<T> Resolve() const
		{
			return std::dynamic_pointer_cast<T>(Resolve_Internal());
		}

	private:
		std::shared_ptr<Object> Resolve_Internal() const;
	};
}