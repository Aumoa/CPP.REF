// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "CoreAPI.h"

#include "CoreConcepts.h"
#include <vector>
#include <functional>

namespace SC::Runtime::Core
{
	template<class T>
	struct TFunction
	{
		static_assert("TFunction must include parameter type that is function like void(TArgs...).");
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
		template<TIsCallable T>
		TFunction(T lambda) : callable(lambda), hash_code()
		{
			hash_code = callable.target_type().hash_code();
		}

		template<class T> requires TIsBaseOf<T, Object>
		TFunction(T* ptr, TRet (T::* callable)(TArgs...)) : This([ptr, callable](TArgs... args)
			{
				(ptr->*callable)(args...);
			})
		{
			std::function<TRet(T*, TArgs...)> wrap = callable;

			hash_code = (size_t)ptr;
			hash_code = hash_code ^ wrap.target_type().hash_code();
		}

		size_t GetHashCode() const
		{
			return hash_code;
		}

		TRet Invoke(TArgs... args) const
		{
			return callable(args...);
		}

		TRet operator()(TArgs... args) const
		{
			return Invoke(args...);
		}
	};

	template<class T, class TFunc> requires TIsBaseOf<T, Object>
	struct TObjectDelegateArgs
	{

	};

	template<class T, class TRet, class... TArgs>
	struct TObjectDelegateArgs<T, TRet(TArgs...)>
	{

	};

	template<class T>
	struct TMulticastDelegate
	{
		static_assert("TMulticastDelegate must include parameter type that is function like void(TArgs...).");
	};

	template<class... TArgs>
	struct TMulticastDelegate<void(TArgs...)>
	{
		using This = TMulticastDelegate;

	private:
		std::vector<TFunction<void(TArgs...)>> functions;

	public:
		template<TIsCallable T>
		void AddLambda(T lambda)
		{
			functions.emplace_back(lambda);
		}

		template<class T> requires TIsBaseOf<T, Object>
		void AddObject(T* ptr, void (T::* callable)(TArgs...))
		{
			functions.emplace_back(ptr, callable);
		}

		template<TIsCallable T>
		void RemoveLambda(T lambda)
		{
			TFunction<void(TArgs...)> wrap = lambda;
			size_t wrap_hash = wrap.GetHashCode();
			RemoveInternal(wrap_hash);
		}

		template<class T> requires TIsBaseOf<T, Object>
		void RemoveObject(T* ptr, void (T::* callable)(TArgs...))
		{
			TFunction<void(TArgs...)> wrap(ptr, callable);
			size_t wrap_hash = wrap.GetHashCode();
			RemoveInternal(wrap_hash);
		}

		void Invoke(TArgs... args) const
		{
			for (auto& item : functions)
			{
				item(args...);
			}
		}

		void operator()(TArgs... args)
		{
			Invoke(args...);
		}

		template<TIsCallable T>
		This& operator +=(T lambda)
		{
			AddLambda(lambda);
			return *this;
		}

		template<TIsCallable T>
		This& operator -=(T lambda)
		{
		}

	private:
		void RemoveInternal(size_t hash_code)
		{
			for (size_t i = 0; i < functions.size(); ++i)
			{
				if (functions[i].GetHashCode() == hash_code)
				{
					functions.erase(functions.begin() + i);
					break;
				}
			}
		}
	};
}