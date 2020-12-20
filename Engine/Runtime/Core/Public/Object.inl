// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include <utility>

namespace SC::Runtime::Core
{
	template<TIsNotPointer T, class... TArgs> requires TIsBaseOf<T, Object> && THasConstructor<T, TArgs...>
	inline TRefPtr<T, true> Object::NewObject(TArgs&&... args)
	{
		auto ptr = new T(std::forward<TArgs>(args)...);
		ptr->bLockCollecting = false;
		TRefPtr<T, true> binder = ptr;
		return binder;
	}
	
	template<TIsNotPointer T, bool bThreadSafe, class... TArgs> requires TIsBaseOf<T, Object> && THasConstructor<T, TArgs...>
	inline TRefPtr<T, bThreadSafe> Object::NewObject(TArgs&&... args)
	{
		auto ptr = new T(std::forward<TArgs>(args)...);
		ptr->bLockCollecting = false;
		TRefPtr<T, bThreadSafe> binder = ptr;
		return binder;
	}
}