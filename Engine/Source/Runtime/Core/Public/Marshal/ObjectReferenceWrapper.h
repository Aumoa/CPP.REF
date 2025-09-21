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
		inline std::shared_ptr<T> AsNative() const
		{
			return std::dynamic_pointer_cast<T>(AsNative_Internal());
		}

		template<class T>
		static ObjectReferenceWrapper FromObject(const std::shared_ptr<T>& obj)
		{
			return obj ? obj->AsWrapper() : ObjectReferenceWrapper{};
		}

	private:
		std::shared_ptr<Object> AsNative_Internal() const;
	};
}