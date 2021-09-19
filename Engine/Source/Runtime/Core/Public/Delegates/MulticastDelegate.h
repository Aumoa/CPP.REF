// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include <functional>
#include <map>
#include <atomic>
#include <concepts>
#include <mutex>
#include <memory>

class DelegateHandle;

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
		MulticastDelegateBase* _delegate = nullptr;
		std::weak_ptr<int64> _holder;
		int64 _id = 0;
	};

private:
	Impl _impl;

public:
	DelegateHandle()
	{
	}

	DelegateHandle(const Impl& impl) : _impl(impl)
	{
	}

	DelegateHandle(const DelegateHandle& rhs)
		: _impl(rhs._impl)
	{
	}

	DelegateHandle(DelegateHandle&& rhs)
		: _impl(std::move(rhs._impl))
	{
	}
	
	~DelegateHandle()
	{
		Reset();
	}

	inline bool IsValid() const
	{
		return !_impl._holder.expired();
	}

	inline void Reset()
	{
		if (IsValid())
		{
			_impl._delegate->Remove(*this);
			_impl._delegate = nullptr;
			_impl._holder.reset();
		}
	}

	DelegateHandle& operator =(const DelegateHandle& rhs)
	{
		Reset();
		_impl = rhs._impl;
		return *this;
	}

	DelegateHandle& operator =(DelegateHandle&& rhs)
	{
		Reset();
		_impl = std::move(rhs._impl);
		return *this;
	}
};

/// <summary>
/// Represents a multicast delegate that is, a delegate that can have more than one element in its invocation list.
/// </summary>
template<class>
class MulticastDelegate
{
};

/// <summary>
/// Represents a multicast delegate that is, a delegate that can have more than one element in its invocation list.
/// </summary>
/// <typeparam name="...TArgs"> The type of function arguments. </typeparam>
template<class... TArgs>
class MulticastDelegate<void(TArgs...)> : public MulticastDelegateBase
{
	using TPayload = std::function<void(TArgs...)>;

	struct DelegateInstance
	{
		TPayload Body;
		std::shared_ptr<int64> Id;

		bool bHolder = false;
		std::weak_ptr<SObject> Holder;

		DelegateInstance(int64 id, TPayload payload)
			: Body(std::move(payload))
			, Id(std::make_shared<int64>(id))
		{
		}

		DelegateInstance(int64 id, TPayload payload, std::weak_ptr<SObject> holder)
			: Body(std::move(payload))
			, Id(std::make_shared<int64>(id))
			, bHolder(true)
			, Holder(std::move(holder))
		{
		}

		template<class... TInvokeArgs>
		bool ApplyAfter(TInvokeArgs&&... args)
		{
			std::shared_ptr<SObject> lock;
			if (bHolder)
			{
				if (Holder.expired())
				{
					return false;
				}

				lock = Holder.lock();
			}

			Body(std::forward<TInvokeArgs>(args)...);
			return true;
		}
	};

	std::mutex _lock;
	std::map<int64, DelegateInstance> _payload;
	int64 _id = 0;

public:
	~MulticastDelegate() noexcept
	{
	}

	/// <summary>
	/// Invoke all functions.
	/// </summary>
	/// <param name="...args"> The function arguments. </param>
	template<class... TInvokeArgs>
	void Invoke(TInvokeArgs&&... args)
	{
		std::unique_lock locker(_lock);

		std::vector<int64> removeList;
		for (auto payload : _payload)
		{
			if (!payload.second.ApplyAfter(std::forward<TInvokeArgs>(args)...))
			{
				removeList.emplace_back(*payload.second.Id);
			}
		}

		for (auto& id : removeList)
		{
			_payload.erase(id);
		}
	}

	/// <summary>
	/// Add raw function to multicast delegate.
	/// </summary>
	/// <param name="func"> The raw function. </param>
	/// <returns> Return valid function id if succeeded to bind, otherwise return -1. </returns>
	DelegateHandle::Impl AddRaw(TPayload func)
	{
		std::unique_lock locker(_lock);
		int64 id = _id++;

		DelegateInstance instance(id, std::move(func));
		DelegateHandle::Impl handle;
		handle._delegate = this;
		handle._holder = instance.Id;
		handle._id = *instance.Id;
		_payload.emplace(id, std::move(instance));
		return handle;
	}

	template<std::derived_from<SObject> U>
	DelegateHandle::Impl AddSObject(U* sobject, void(U::*payload)(TArgs...))
	{
		std::unique_lock locker(_lock);
		int64 id = _id++;

		DelegateInstance instance(id, [sobject, payload](TArgs&&... args)
			{
				(sobject->*payload)(std::forward<TArgs>(args)...);
			}, sobject->weak_from_this());
		DelegateHandle::Impl handle;
		handle._delegate = this;
		handle._holder = instance.Id;
		handle._id = *instance.Id;
		_payload.emplace(id, std::move(instance));
		return handle;
	}

	/// <summary>
	/// Remove binded function using delegate id.
	/// </summary>
	virtual void Remove(DelegateHandle& handle) override
	{
		std::unique_lock locker(_lock);
		if (handle.IsValid())
		{
			_payload.erase(handle._impl._id);
		}
	}

	DelegateHandle::Impl operator +=(TPayload func)
	{
		return AddRaw(std::move(func));
	}
};

#define DECLARE_MULTICAST_DELEGATE(DelegateName, ...)				\
	using DelegateName = MulticastDelegate<void(__VA_ARGS__)>;