// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <functional>
#include <map>
#include <atomic>
#include <concepts>
#include <mutex>
#include <memory>
#include "GC/WeakPtr.h"

class DelegateHandle;
class SObject;

class MulticastDelegateBase
{
public:
	virtual void Remove(DelegateHandle& handle) = 0;
};

class DelegateHandle
{
	template<class>
	friend class MulticastDelegate;

public:
	struct Impl
	{
		MulticastDelegateBase* Delegate = nullptr;
		std::weak_ptr<int64> Holder;
		int64 Id = 0;
	};

private:
	Impl _Impl;

public:
	DelegateHandle()
	{
	}

	DelegateHandle(const Impl& InImpl) : _Impl(InImpl)
	{
	}

	DelegateHandle(const DelegateHandle& Rhs)
		: _Impl(Rhs._Impl)
	{
	}

	DelegateHandle(DelegateHandle&& Rhs)
		: _Impl(std::move(Rhs._Impl))
	{
	}
	
	~DelegateHandle()
	{
		Reset();
	}

	inline bool IsValid() const
	{
		return !_Impl.Holder.expired();
	}

	inline void Reset()
	{
		if (IsValid())
		{
			_Impl.Delegate->Remove(*this);
			_Impl.Delegate = nullptr;
			_Impl.Holder.reset();
		}
	}

	DelegateHandle& operator =(const DelegateHandle& Rhs)
	{
		Reset();
		_Impl = Rhs._Impl;
		return *this;
	}

	DelegateHandle& operator =(DelegateHandle&& Rhs)
	{
		Reset();
		_Impl = std::move(Rhs._Impl);
		return *this;
	}
};

template<class>
class MulticastDelegate
{
};

template<class... TArgs>
class MulticastDelegate<void(TArgs...)> : public MulticastDelegateBase
{
	using TPayload = std::function<void(TArgs...)>;

	struct DelegateInstance
	{
		TPayload Body;
		std::shared_ptr<int64> Id;

		bool bHolder = false;
		WeakPtr<SObject> Holder;

		DelegateInstance(int64 id, TPayload payload)
			: Body(std::move(payload))
			, Id(std::make_shared<int64>(id))
		{
		}

		DelegateInstance(int64 id, TPayload payload, WeakPtr<SObject> holder)
			: Body(std::move(payload))
			, Id(std::make_shared<int64>(id))
			, bHolder(true)
			, Holder(std::move(holder))
		{
		}

		template<class... TInvokeArgs>
		bool ApplyAfter(TInvokeArgs&&... InArgs)
		{
			if (bHolder)
			{
				if (!Holder.IsValid())
				{
					return false;
				}
			}

			Body(std::forward<TInvokeArgs>(InArgs)...);
			return true;
		}
	};

	std::mutex Mtx;
	std::map<int64, DelegateInstance> Payload;
	int64 Id = 0;

public:
	~MulticastDelegate() noexcept
	{
	}

	template<class... TInvokeArgs>
	void Broadcast(TInvokeArgs&&... InArgs)
	{
		std::unique_lock Mtx_lock(Mtx);

		std::vector<int64> CompactList;
		for (auto Item : Payload)
		{
			if (!Item.second.ApplyAfter(std::forward<TInvokeArgs>(InArgs)...))
			{
				CompactList.emplace_back(*Item.second.Id);
			}
		}

		for (auto& Id : CompactList)
		{
			Payload.erase(Id);
		}
	}

	void Clear()
	{
		std::unique_lock Mtx_lock(Mtx);
		Payload.clear();
		Id = 0;
	}

	DelegateHandle::Impl AddRaw(TPayload Body)
	{
		std::unique_lock Mtx_lock(Mtx);
		int64 Myid = Id++;

		DelegateInstance instance(Myid, std::move(Body));
		DelegateHandle::Impl handle;
		handle.Delegate = this;
		handle.Holder = instance.Id;
		handle.Id = *instance.Id;
		Payload.emplace(Myid, std::move(instance));
		return handle;
	}

	template<std::derived_from<SObject> U>
	DelegateHandle::Impl AddSObject(U* Object, void(U::*Body)(TArgs...))
	{
		std::unique_lock Mtx_lock(Mtx);
		int64 Myid = Id++;

		DelegateInstance Instance(Myid, [Object, Body](TArgs&&... InArgs)
		{
			(Object->*Body)(std::forward<TArgs>(InArgs)...);
		}, Object);
		DelegateHandle::Impl Handle;
		Handle.Delegate = this;
		Handle.Holder = Instance.Id;
		Handle.Id = *Instance.Id;
		Payload.emplace(Myid, std::move(Instance));
		return Handle;
	}

	virtual void Remove(DelegateHandle& Handle) override
	{
		std::unique_lock Mtx_lock(Mtx);
		if (Handle.IsValid())
		{
			Payload.erase(Handle._Impl.Id);
		}
	}

	void RemoveAll(SObject* InObject)
	{
		std::unique_lock Mtx_lock(Mtx);
		std::vector<decltype(Payload.begin())> Compacts;

		for (auto It = Payload.begin(); It != Payload.end(); ++It)
		{
			auto& [Key, Value] = *It;
			if (Value.bHolder)
			{
				if (Value.Holder.IsValid() || Value.Holder.Get() == InObject)
				{
					Compacts.emplace_back(It);
					continue;
				}
			}
		}

		for (auto& It : Compacts)
		{
			Payload.erase(It);
		}
	}

	DelegateHandle::Impl operator +=(TPayload Body)
	{
		return AddRaw(std::move(Body));
	}
};

#define DECLARE_MULTICAST_DELEGATE(DelegateName, ...)				\
	using DelegateName = MulticastDelegate<void(__VA_ARGS__)>;