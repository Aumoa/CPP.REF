// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

namespace SC::Runtime::Core
{
	template<class T, class... TArgs> requires TIsNotPointer<T> && TIsBaseOf<T, Object> && THasConstructor<T, TArgs...>
	inline TRefPtr<T> Object::NewObject(TArgs... args)
	{
		auto ptr = new T(args...);
		ptr->bLockCollecting = false;
		TRefPtr<T> binder = ptr;
		return binder;
	}
}