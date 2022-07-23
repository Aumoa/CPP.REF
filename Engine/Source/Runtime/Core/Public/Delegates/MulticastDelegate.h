// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "IMulticastDelegate.h"
#include "DelegateHandle.h"
#include "Threading/Spinlock.h"
#include "CoreObject/WeakPtr.h"
#include <functional>
#include <map>
#include <atomic>
#include <concepts>
#include <memory>

template<class>
class MulticastDelegate
{
};

template<class... TArgs>
class MulticastDelegate<void(TArgs...)> : public IMulticastDelegate
{
	using TPayload = std::function<void(TArgs...)>;

	struct DelegateInstance
	{
		TPayload Body;
		std::shared_ptr<int64> Id;
		std::function<bool()> Holder;

		DelegateInstance(int64 id, TPayload payload)
			: Body(std::move(payload))
			, Id(std::make_shared<int64>(id))
		{
		}

		DelegateInstance(int64 id, TPayload payload, std::function<bool()> holder)
			: Body(std::move(payload))
			, Id(std::make_shared<int64>(id))
			, Holder(std::move(holder))
		{
		}

		template<class... TInvokeArgs>
		bool Invoke(TInvokeArgs&&... InArgs)
		{
			if (Holder && !Holder())
			{
				return false;
			}

			Body(std::forward<TInvokeArgs>(InArgs)...);
			return true;
		}
	};

	Spinlock _lock;
	std::map<int64, DelegateInstance> _payload;
	int64 _id = 0;

public:
	MulticastDelegate() = default;

	template<class... TInvokeArgs>
	void Broadcast(TInvokeArgs&&... args) requires
#if __INTELLISENSE__
		true
#else
		std::invocable<TPayload, TInvokeArgs>
#endif
	{
		std::vector<int64> compacts;
		std::vector<DelegateInstance> invokes;

		{
			auto lock = std::unique_lock<Spinlock>(_lock, Spinlock::Readonly);
			invokes.reserve(_payload.size());
			for (auto& instance : _payload)
			{
				invokes.emplace_back(instance.second);
			}
		}

		for (auto& invoke : invokes)
		{
			if (!invoke.Invoke(std::forward<TInvokeArgs>(args)...))
			{
				compacts.emplace_back(*invoke.Id);
			}
		}

		if (compacts.size())
		{
			std::unique_lock lock(_lock);
			for (auto& compact : compacts)
			{
				_payload.erase(compact);
			}
		}
	}

	void Clear()
	{
		std::unique_lock lock(_lock);
		_payload.clear();
		_id = 0;
	}

private:

public:
	template<class T>
	static inline DelegateInstance CreateLambda(T&& body) requires
		std::invocable<T, TArgs...>
	{
		return DelegateInstance(0, std::forward<T>(body));
	}

	template<class T>
	static inline DelegateInstance CreateWeakLambda(Object* self, T&& body) requires
		std::invocable<T, TArgs...>
	{
		return DelegateInstance(0, std::forward<T>(body), ImplGetHolder(self));
	}

	template<class TSelf, class T>
	static inline DelegateInstance CreateWeakLambda(const std::shared_ptr<TSelf>& self, T&& body) requires
		std::invocable<T, TArgs...>
	{
		return DelegateInstance(0, std::forward<T>(body), ImplGetHolder(self));
	}

	template<class TSelf, class... TInvokeArgs>
	static inline DelegateInstance CreateObject(TSelf* self, void (TSelf::*func)(TInvokeArgs...)) requires
		std::derived_from<TSelf, Object>
	{
		return CreateWeakLambda(self, [self, func](TArgs... args) { (self->*func)(args...); });
	}

	template<class T>
	inline ScopedDelegateHolder AddLambda(T&& body) requires
		std::invocable<T, TArgs...>
	{
		return Add(CreateLambda(std::forward<T>(body)));
	}

	template<class T>
	inline ScopedDelegateHolder AddWeakLambda(Object* self, T&& body) requires
		std::invocable<T, TArgs...>
	{
		return Add(CreateWeakLambda(self, std::forward<T>(body)));
	}

	template<class TSelf, class T>
	inline ScopedDelegateHolder AddWeakLambda(const std::shared_ptr<TSelf>& self, T&& body) requires
		std::invocable<T, TArgs...>
	{
		return Add(CreateWeakLambda(self, std::forward<T>(body)));
	}

	template<class TSelf, class... TInvokeArgs>
	inline ScopedDelegateHolder AddObject(TSelf* self, void (TSelf::*func)(TInvokeArgs...)) requires
		std::derived_from<TSelf, Object>
	{
		return Add(CreateObject(self, func));
	}

	inline ScopedDelegateHolder Add(DelegateInstance&& instance)
	{
		std::unique_lock lock(_lock);
		*instance.Id = _id++;
		ScopedDelegateHolder handle(this, instance.Id);
		_payload.emplace(*instance.Id, std::move(instance));
		return handle;
	}

	virtual void Remove(DelegateHandle& handle) override
	{
		std::unique_lock lock(_lock);
		if (handle.IsValid())
		{
			_payload.erase(handle._delegate._id);
			handle._delegate.Reset();
		}
	}

	template<class T>
	inline auto operator +=(T&& body) requires
		std::invocable<T, TArgs...>
	{
		return AddLambda(std::forward<T>(body));
	}

	inline auto operator +=(DelegateInstance&& instance)
	{
		return Add(std::move(instance));
	}

	inline auto operator -=(DelegateHandle& handle)
	{
		return Remove(handle);
	}

private:
	template<class U>
	static auto ImplGetHolder(U* object) requires
		std::derived_from<U, Object>
	{
		return [self = WeakPtr(object)]
		{
			return self.IsValid();
		};
	}

	template<class U>
	static auto ImplGetHolder(const std::shared_ptr<U>& object)
	{
		return [self = std::weak_ptr(object)]
		{
			return !self.expired();
		};
	}

	template<class U>
	static auto ImplGetHolder(U* object)
	{
		return nullptr;
	}
};

#define DECLARE_MULTICAST_DELEGATE(DelegateName, ...)				\
	using DelegateName = ::MulticastDelegate<void(__VA_ARGS__)>;
