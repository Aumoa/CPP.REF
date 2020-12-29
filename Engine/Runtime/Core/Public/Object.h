// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include "WeakReferences.h"

namespace SC::Runtime::Core
{
	class Object;
	class String;

	template<TIsNotPointer T, bool bThreadSafe>
	class TRefPtr;
	template<TIsNotPointer T, bool bThreadSafe>
	class TWeakPtr;

	class CORE_API Object
	{
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

		bool operator ==(const TRefPtr<Object, true>& right) const;
		bool operator !=(const TRefPtr<Object, true>& right) const;

	private:
		void AddRef();
		void Release();
		void AddRefInterlocked();
		void ReleaseInterlocked();
		WeakReferences* GetWeakReferences() const;
	};
}

#include "Object.inl"