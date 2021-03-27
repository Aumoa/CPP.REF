// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <utility>

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

template<TIsNotPointer O, TIsNotPointer T> requires TIsBaseOf<O, Object> && TIsBaseOf<T, Object>
inline O* Object::Cast(T* ptr)
{
	return dynamic_cast<O*>(ptr);
}

template<TIsNotPointer O, TIsNotPointer T> requires TIsBaseOf<O, Object> && TIsBaseOf<T, Object>
inline const O* Object::Cast(const T* ptr)
{
	return dynamic_cast<const O*>(ptr);
}