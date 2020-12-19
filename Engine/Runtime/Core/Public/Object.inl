// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include <utility>

namespace SC::Runtime::Core
{
	template<TIsNotPointer T, class... TArgs> requires TIsBaseOf<T, Object> && THasConstructor<T, TArgs...>
	inline TRefPtr<T> Object::NewObject(TArgs&&... args)
	{
		auto ptr = new T(std::forward<TArgs>(args)...);
		ptr->bLockCollecting = false;
		TRefPtr<T> binder = ptr;
		return binder;
	}
}