// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "System/Delegate.h"
#include "System/Referencer.h"
#include "System/DelegateHandle.h"
#include "Threading/Spinlock.h"
#include "System/AssertionMacros.h"
#include "Linq/Any.h"
#include <unordered_map>

template<class T>
class MulticastDelegate
{
	static_assert(std::same_as<T, std::function<void>>, "invalid template argument.");
};

template<class... TArgs>
class MulticastDelegate<void(TArgs...)>
{
	MulticastDelegate(const MulticastDelegate&) = delete;
	MulticastDelegate(MulticastDelegate&&) = delete;

public:
	using Delegate_t = ::Delegate<void(TArgs...)>;

private:
	Spinlock Lock;
	std::unordered_map<int64, Delegate_t> Invocables;
	int64 UniqueInstanceId = 0;
	int64 Id = 0;

public:
	MulticastDelegate()
	{
		static volatile int64 InstanceIdAllocator = 0;
		UniqueInstanceId = PlatformAtomics::InterlockedIncrement(&InstanceIdAllocator);
	}

	~MulticastDelegate() noexcept
	{
	}

	template<class... TInvokeArgs>
	void Broadcast(TInvokeArgs&&... InArgs)
	{
		static thread_local std::vector<int64> Compacts;
		Compacts.clear();
		Compacts.reserve(Invocables.size());

		{
			std::shared_lock ScopedLock(Lock);
			for (auto& [InvokeId, Invoke] : Invocables)
			{
				if (Invoke.SafeInvoke(std::forward<TInvokeArgs>(InArgs)...).HasValue() == false)
				{
					Compacts.emplace_back(InvokeId);
				}
			}
		}

		if (Compacts | Linq::Any())
		{
			std::unique_lock ScopedLock(Lock);
			for (auto& Compact : Compacts)
			{
				Invocables.erase(Compact);
			}
		}
	}

	void Clear() noexcept
	{
		std::unique_lock ScopedLock(Lock);
		Invocables.clear();
	}

	void Remove(DelegateHandle& Handle)
	{
		if (ensure(Handle.InstanceId == UniqueInstanceId) == false)
		{
			return;
		}

		std::unique_lock ScopedLock(Lock);
		Invocables.erase(Handle.Id);

		ScopedLock.unlock();
		Handle.Reset();
	}

public:
	DelegateHandle Add(Delegate_t Dele)
	{
		std::unique_lock ScopedLock(Lock);
		int64 MyId = ++Id;
		Invocables.emplace(MyId, std::move(Dele));
		return DelegateHandle(UniqueInstanceId, MyId);
	}

	template<class U>
	DelegateHandle AddLambda(U&& Val) requires requires { Delegate_t::CreateLambda(std::forward<U>(Val)); }
	{
		return Add(Delegate_t::CreateLambda(std::forward<U>(Val)));
	}

	DelegateHandle AddStatic(void(*Val)(TArgs...))
	{
		return Add(Delegate_t::CreateStatic(Val));
	}

	template<class UObject, class URef = std::remove_pointer_t<std::remove_reference_t<UObject>>>
	DelegateHandle AddRaw(UObject&& Obj, void(URef::*Val)(TArgs...)) requires requires { Delegate_t::CreateRaw(std::forward<UObject>(Obj), Val); }
	{
		return Add(Delegate_t::CreateRaw(std::forward<UObject>(Obj), Val));
	}

	template<class UObject, class ULambda>
	DelegateHandle AddWeakLambda(const UObject& Obj, ULambda&& Val) requires requires { Delegate_t::CreateWeakLambda(Obj, Val); }
	{
		return Add(Delegate_t::CreateWeakLambda(Obj, std::forward<ULambda>(Val)));
	}

	template<class UObject, class URef = std::remove_pointer_t<std::remove_reference_t<UObject>>>
	DelegateHandle AddObject(UObject&& Obj, void(URef::*Val)(TArgs...)) requires requires { Delegate_t::CreateObject(std::forward<UObject>(Obj), Val); }
	{
		return Add(Delegate_t::CreateObject(std::forward<UObject>(Obj), Val));
	}

	DelegateHandle operator +=(Delegate_t Dele)
	{
		return Add(std::move(Dele));
	}

	template<class U>
	DelegateHandle operator +=(U&& Val) requires requires { Delegate_t::CreateLambda(std::forward<U>(Val)); }
	{
		return Add(Delegate_t::CreateLambda(std::forward<U>(Val)));
	}

	DelegateHandle operator +=(void(*Val)(TArgs...))
	{
		return Add(Delegate_t::CreateStatic(Val));
	}
};

#define DECLARE_MULTICAST_DELEGATE(DelegateName, ...) using DelegateName = MulticastDelegate<void(__VA_ARGS__)>