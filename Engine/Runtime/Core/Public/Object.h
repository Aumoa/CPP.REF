// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"
#include "CoreConcepts.h"

namespace SC::Runtime::Core
{
	class Object;
	class String;

	template<TIsRefCore T>
	class TRefPtr;

	class CORE_API Object
	{
		template<TIsRefCore T>
		friend class TRefPtr;

	private:
		bool bLockCollecting : 1;
		size_t ref_count;

	public:
		Object();
		virtual ~Object();

		virtual TRefPtr<String> ToString() const;
		virtual bool Equals(TRefPtr<Object> right) const;
		virtual size_t GetHashCode() const;

		template<class T, class... TArgs> requires TIsBaseOf<T, Object>
		static TRefPtr<T> NewObject(TArgs... args)
		{
			auto ptr = new T(args...);
			ptr->bLockCollecting = false;
			TRefPtr<T> binder = ptr;
			return binder;
		}

		bool operator ==(const TRefPtr<Object>& right) const;
		bool operator !=(const TRefPtr<Object>& right) const;

	private:
		void AddRef();
		void Release();
	};
}