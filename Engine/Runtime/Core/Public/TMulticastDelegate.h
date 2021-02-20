// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"

#include "CoreConcepts.h"
#include <vector>
#include <functional>

template<TIsNotPointer T, bool bThreadSafe>
class TRefPtr;

template<class T>
struct TFunction
{

};

template<class TRet, class... TArgs>
struct TFunction<TRet(TArgs...)>
{
public:
	using This = TFunction;

private:
	std::function<TRet(TArgs...)> callable;
	size_t hash_code;

public:
	inline TFunction();

	template<TIsCallable T>
	inline TFunction(T lambda);

	template<class T>
	inline TFunction(const T* ptr, TRet(T::* callable)(TArgs...));

	inline size_t GetHashCode() const;
	inline TRet Invoke(TArgs... args) const;

	vs_property_get(bool, IsValid);
	inline bool IsValid_get() const;

	inline TRet operator()(TArgs... args) const;
};

template<class T>
struct TMulticastDelegate
{

};

template<TIsNotPointer T, class... TArgs>
struct TObjectFunctionBind
{
	TRefPtr<T, true> ptr;
	void (T::* callable)(TArgs...);

public:
	inline TObjectFunctionBind(TRefPtr<T, true> ptr, void (T::* callable)(TArgs...));
	inline TObjectFunctionBind(const T* ptr, void (T::* callable)(TArgs...));
};

template<TIsNotPointer T, class... TArgs>
struct TRawFunctionBind
{
	const T* ptr;
	void (T::* callable)(TArgs...);

public:
	inline TRawFunctionBind(const T* ptr, void (T::* callable)(TArgs...));
};

template<class... TArgs>
struct TMulticastDelegate<void(TArgs...)>
{
	using This = TMulticastDelegate;

private:
	std::vector<TFunction<void(TArgs...)>> functions;
	std::vector<TRefPtr<Object, true>> objects;

public:
	template<TIsCallable T>
	inline void AddLambda(T lambda);
	template<TIsNotPointer T>
	inline void AddObject(TRefPtr<T, true> ptr, void (T::* callable)(TArgs...));
	template<TIsNotPointer T>
	inline void AddObject(const T* ptr, void (T::* callable)(TArgs...));
	template<TIsNotPointer T>
	inline void AddRaw(const T* ptr, void (T::* callable)(TArgs...));

	template<TIsCallable T>
	inline void RemoveLambda(T lambda);
	template<TIsNotPointer T>
	inline void RemoveObject(TRefPtr<T, true> ptr, void (T::* callable)(TArgs...));
	template<TIsNotPointer T>
	inline void RemoveObject(const T* ptr, void (T::* callable)(TArgs...));
	template<TIsNotPointer T>
	inline void RemoveRaw(const T* ptr, void (T::* callable)(TArgs...));

	inline void Invoke(TArgs... args) const;
	inline size_t NumBindings() const;

	inline void operator()(TArgs... args) const;

	template<TIsCallable T>
	inline This& operator +=(T lambda);
	template<TIsNotPointer T>
	inline This& operator +=(const TObjectFunctionBind<T, TArgs...>& object_bind);
	template<TIsNotPointer T>
	inline This& operator +=(const TRawFunctionBind<T, TArgs...>& raw_bind);
	template<TIsCallable T>
	inline This& operator -=(T lambda);
	template<TIsNotPointer T>
	inline This& operator -=(const TObjectFunctionBind<T, TArgs...>& object_bind);
	template<TIsNotPointer T>
	inline This& operator -=(const TRawFunctionBind<T, TArgs...>& raw_bind);

private:
	inline void RemoveInternal(size_t hash_code);
};

#include "TMulticastDelegate.inl"