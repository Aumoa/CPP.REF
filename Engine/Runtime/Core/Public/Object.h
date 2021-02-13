// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include "WeakReferences.h"

class Object;
class String;

template<TIsNotPointer T, bool bThreadSafe>
class TRefPtr;
template<TIsNotPointer T, bool bThreadSafe>
class TWeakPtr;

class CORE_API Object
{
public:
	using This = Object;

private:
	template<TIsNotPointer T, bool bThreadSafe>
	friend class TRefPtr;
	template<TIsNotPointer T, bool bThreadSafe>
	friend class TWeakPtr;
		
	Object(const Object&) = delete;
	Object(Object&&) = delete;
	Object& operator =(const Object&) = delete;
	Object& operator =(Object&&) = delete;

private:
	bool bLockCollecting : 1;
	size_t ref_count;
	mutable WeakReferences* weak_references;

public:
	Object();
	virtual ~Object();

	virtual TRefPtr<String, true> ToString() const;
	virtual bool Equals(TRefPtr<Object, true> right) const;
	virtual size_t GetHashCode() const;

	template<TIsNotPointer T, class... TArgs> requires TIsBaseOf<T, Object> && THasConstructor<T, TArgs...>
	inline static TRefPtr<T, true> NewObject(TArgs&&... args);
	template<TIsNotPointer T, bool bThreadSafe, class... TArgs> requires TIsBaseOf<T, Object> && THasConstructor<T, TArgs...>
	inline static TRefPtr<T, bThreadSafe> NewObject(TArgs&&... args);
	template<TIsNotPointer O, TIsNotPointer T, bool bThreadSafe> requires TIsBaseOf<O, Object> && TIsBaseOf<T, Object>
	inline static TRefPtr<O, bThreadSafe> Cast(TRefPtr<T, bThreadSafe> ptr);
	template<TIsNotPointer O, TIsNotPointer T> requires TIsBaseOf<O, Object>&& TIsBaseOf<T, Object>
	inline static O* Cast(T* ptr);
	template<TIsNotPointer O, TIsNotPointer T> requires TIsBaseOf<O, Object>&& TIsBaseOf<T, Object>
	inline static const O* Cast(const T* ptr);


	bool operator ==(const TRefPtr<Object, true>& right) const;
	bool operator !=(const TRefPtr<Object, true>& right) const;

private:
	void AddRef();
	void Release();
	void AddRefInterlocked();
	void ReleaseInterlocked();
	WeakReferences* GetWeakReferences() const;
};

#include "Object.inl"