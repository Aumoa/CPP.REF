// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

template<class TRet, class... TArgs>
inline TFunction<TRet(TArgs...)>::TFunction() : callable(nullptr), hash_code(0)
{

}

template<class TRet, class... TArgs>
template<TIsCallable T>
inline TFunction<TRet(TArgs...)>::TFunction(T lambda) : callable(lambda), hash_code(0)
{
	hash_code = callable.target_type().hash_code();
}

template<class TRet, class... TArgs>
template<class T>
inline TFunction<TRet(TArgs...)>::TFunction(const T* ptr, TRet(T::* callable)(TArgs...))
	: callable([ptr, callable](TArgs... args)
	{
		(const_cast<T*>(ptr)->*callable)(args...);
	})
	, hash_code(0)
{
	hash_code = this->callable.target_type().hash_code() ^ (size_t)ptr;
}

template<class TRet, class... TArgs>
inline size_t TFunction<TRet(TArgs...)>::GetHashCode() const
{
	return hash_code;
}

template<class TRet, class... TArgs>
inline TRet TFunction<TRet(TArgs...)>::Invoke(TArgs... args) const
{
	return callable(args...);
}

template<class TRet, class... TArgs>
inline bool TFunction<TRet(TArgs...)>::IsValid_get() const
{
	return (bool)callable;
}

template<class TRet, class... TArgs>
inline TRet TFunction<TRet(TArgs...)>::operator ()(TArgs... args) const
{
	return callable(args...);
}

template<TIsNotPointer T, class... TArgs>
inline TObjectFunctionBind<T, TArgs...>::TObjectFunctionBind(TRefPtr<T, true> ptr, void (T::* callable)(TArgs...))
	: ptr(std::move(ptr))
	, callable(callable)
{

}

template<TIsNotPointer T, class... TArgs>
inline TObjectFunctionBind<T, TArgs...>::TObjectFunctionBind(const T* ptr, void (T::* callable)(TArgs...))
	: ptr(ptr)
	, callable(callable)
{

}

template<TIsNotPointer T, class... TArgs>
inline TRawFunctionBind<T, TArgs...>::TRawFunctionBind(const T* ptr, void (T::* callable)(TArgs...))
	: ptr(ptr)
	, callable(callable)
{

}

template<class... TArgs>
template<TIsCallable T>
inline void TMulticastDelegate<void(TArgs...)>::AddLambda(T lambda)
{
	functions.emplace_back(lambda);
	objects.emplace_back();
}

template<class... TArgs>
template<TIsNotPointer T>
inline void TMulticastDelegate<void(TArgs...)>::AddObject(TRefPtr<T> ptr, void (T::* callable)(TArgs...))
{
	functions.emplace_back(ptr.Get(), callable);
	objects.emplace_back(std::move(ptr));
}

template<class... TArgs>
template<TIsNotPointer T>
inline void TMulticastDelegate<void(TArgs...)>::AddObject(const T* ptr, void (T::* callable)(TArgs...))
{
	functions.emplace_back(ptr, callable);
	objects.emplace_back(ptr);
}

template<class... TArgs>
template<TIsNotPointer T>
inline void TMulticastDelegate<void(TArgs...)>::AddRaw(const T* ptr, void (T::* callable)(TArgs...))
{
	functions.emplace_back(ptr, callable);
	objects.emplace_back();
}

template<class... TArgs>
template<TIsCallable T>
inline void TMulticastDelegate<void(TArgs...)>::RemoveLambda(T lambda)
{
	TFunction<void(TArgs...)> wrap = lambda;
	size_t wrap_hash = wrap.GetHashCode();
	RemoveInternal(wrap_hash);
}

template<class... TArgs>
template<TIsNotPointer T>
inline void TMulticastDelegate<void(TArgs...)>::RemoveObject(TRefPtr<T> ptr, void (T::* callable)(TArgs...))
{
	TFunction<void(TArgs...)> wrap(ptr.Get(), callable);
	size_t wrap_hash = wrap.GetHashCode();
	RemoveInternal(wrap_hash);
}

template<class... TArgs>
template<TIsNotPointer T>
inline void TMulticastDelegate<void(TArgs...)>::RemoveObject(const T* ptr, void (T::* callable)(TArgs...))
{
	TFunction<void(TArgs...)> wrap(ptr, callable);
	size_t wrap_hash = wrap.GetHashCode();
	RemoveInternal(wrap_hash);
}

template<class... TArgs>
template<TIsNotPointer T>
inline void TMulticastDelegate<void(TArgs...)>::RemoveRaw(const T* ptr, void (T::* callable)(TArgs...))
{
	TFunction<void(TArgs...)> wrap(ptr, callable);
	size_t wrap_hash = wrap.GetHashCode();
	RemoveInternal(wrap_hash);
}

template<class... TArgs>
inline void TMulticastDelegate<void(TArgs...)>::Invoke(TArgs... args) const
{
	for (auto& item : functions)
	{
		item(args...);
	}
}

template<class... TArgs>
inline size_t TMulticastDelegate<void(TArgs...)>::NumBindings() const
{
	return functions.size();
}

template<class... TArgs>
inline void TMulticastDelegate<void(TArgs...)>::operator ()(TArgs... args) const
{
	for (auto& item : functions)
	{
		item(args...);
	}
}

template<class... TArgs>
template<TIsCallable T>
inline auto TMulticastDelegate<void(TArgs...)>::operator +=(T lambda) -> This&
{
	AddLambda(lambda);
	return *this;
}

template<class... TArgs>
template<TIsNotPointer T>
inline auto TMulticastDelegate<void(TArgs...)>::operator +=(const TObjectFunctionBind<T, TArgs...>& object_bind) -> This&
{
	AddObject(std::move(object_bind.ptr), object_bind.callable);
	return *this;
}

template<class... TArgs>
template<TIsNotPointer T>
inline auto TMulticastDelegate<void(TArgs...)>::operator +=(const TRawFunctionBind<T, TArgs...>& raw_bind) -> This&
{
	AddRaw(raw_bind.ptr, raw_bind.callable);
	return *this;
}

template<class... TArgs>
template<TIsCallable T>
inline auto TMulticastDelegate<void(TArgs...)>::operator -=(T lambda) -> This&
{
	RemoveLambda(lambda);
	return *this;
}

template<class... TArgs>
template<TIsNotPointer T>
inline auto TMulticastDelegate<void(TArgs...)>::operator -=(const TObjectFunctionBind<T, TArgs...>& object_bind) -> This&
{
	RemoveObject(std::move(object_bind.ptr), object_bind.callable);
	return *this;
}

template<class... TArgs>
template<TIsNotPointer T>
inline auto TMulticastDelegate<void(TArgs...)>::operator -=(const TRawFunctionBind<T, TArgs...>& raw_bind) -> This&
{
	RemoveRaw(raw_bind.ptr, raw_bind.callable);
	return *this;
}

template<class... TArgs>
inline void TMulticastDelegate<void(TArgs...)>::RemoveInternal(size_t hash_code)
{
	for (size_t i = 0; i < functions.size(); ++i)
	{
		if (functions[i].GetHashCode() == hash_code)
		{
			functions.erase(functions.begin() + i);
			objects.erase(objects.begin() + i);
			break;
		}
	}
}