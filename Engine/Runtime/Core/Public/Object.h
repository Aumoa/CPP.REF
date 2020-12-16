// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

#include "WeakReferences.h"

namespace SC::Runtime::Core
{
	class Object;
	class String;

	template<TIsNotPointer T>
	class TRefPtr;
	template<TIsNotPointer T>
	class TWeakPtr;

	class CORE_API Object
	{
		template<TIsNotPointer T>
		friend class TRefPtr;
		template<TIsNotPointer T>
		friend class TWeakPtr;

	private:
		bool bLockCollecting : 1;
		size_t ref_count;
		mutable WeakReferences* weak_references;

	public:
		Object();
		virtual ~Object();

		virtual TRefPtr<String> ToString() const;
		virtual bool Equals(TRefPtr<Object> right) const;
		virtual size_t GetHashCode() const;

		template<class T, class... TArgs> requires TIsNotPointer<T> && TIsBaseOf<T, Object> && THasConstructor<T, TArgs...>
		inline static TRefPtr<T> NewObject(TArgs... args);

		bool operator ==(const TRefPtr<Object>& right) const;
		bool operator !=(const TRefPtr<Object>& right) const;

	private:
		void AddRef();
		void Release();
		WeakReferences* GetWeakReferences() const;
	};
}

#include "Object.inl"