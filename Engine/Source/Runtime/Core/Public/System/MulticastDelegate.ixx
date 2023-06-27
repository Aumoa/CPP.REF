// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:MulticastDelegate;

export import Core.Std;
export import :IMulticastDelegate;
export import :DelegateHandle;

export template<class>
class MulticastDelegate
{
};

export template<class... TArgs>
class MulticastDelegate<void(TArgs...)> : public IMulticastDelegate
{
	using TPayload = std::function<void(TArgs...)>;

	struct DelegateInstance
	{
		TPayload Body;
		std::shared_ptr<int64> Id;
		std::function<bool()> Holder;

		DelegateInstance(int64 Id, TPayload Payload)
			: Body(std::move(Payload))
			, Id(std::make_shared<int64>(Id))
		{
		}

		DelegateInstance(int64 Id, TPayload Payload, std::function<bool()> Holder)
			: Body(std::move(Payload))
			, Id(std::make_shared<int64>(Id))
			, Holder(std::move(Holder))
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

	std::mutex Lock;
	std::map<int64, DelegateInstance> Payload;
	int64 Id = 0;

public:
	MulticastDelegate() = default;

	template<class... TInvokeArgs>
	void Broadcast(TInvokeArgs&&... InArgs)
	{
		std::vector<int64> Compacts;
		std::vector<DelegateInstance> Invokes;

		{
			auto Lock = std::unique_lock(Lock.AsReadonly());
			Invokes.reserve(Payload.size());
			for (auto& Instance : Payload)
			{
				Invokes.emplace_back(Instance.second);
			}
		}

		for (auto& Invoke : Invokes)
		{
			if (!Invoke.Invoke(std::forward<TInvokeArgs>(InArgs)...))
			{
				Compacts.emplace_back(*Invoke.Id);
			}
		}

		if (Compacts.size())
		{
			std::unique_lock Scope(Lock);
			for (auto& Compact : Compacts)
			{
				Payload.erase(Compact);
			}
		}
	}

	void Clear()
	{
		std::unique_lock lock(Lock);
		Payload.clear();
		Id = 0;
	}

public:
	template<class T>
	static inline DelegateInstance CreateLambda(T&& InBody) requires
		std::invocable<T, TArgs...>
	{
		return DelegateInstance(0, std::forward<T>(InBody));
	}

	template<class TSelf, class T>
	static inline DelegateInstance CreateWeakLambda(const std::shared_ptr<TSelf>& Self, T&& InBody) requires
		std::invocable<T, TArgs...>
	{
		return DelegateInstance(0, std::forward<T>(InBody), ImplGetHolder(Self));
	}

	template<class TSelf, class... TInvokeArgs>
	static inline DelegateInstance CreateShared(TSelf* Self, void (TSelf::*InFunction)(TInvokeArgs...)) requires
		requires { { Self->shared_from_this() }; }
	{
		return CreateWeakLambda(Self->shared_from_this(), [Self, InFunction](TArgs... InArgs) { (Self->*InFunction)(InArgs...); });
	}

	template<class T>
	inline ScopedDelegateHolder AddLambda(T&& InBody) requires
		std::invocable<T, TArgs...>
	{
		return Add(CreateLambda(std::forward<T>(InBody)));
	}

	template<class TSelf, class T>
	inline ScopedDelegateHolder AddWeakLambda(const std::shared_ptr<TSelf>& Self, T&& InBody) requires
		std::invocable<T, TArgs...>
	{
		return Add(CreateWeakLambda(Self, std::forward<T>(InBody)));
	}

	template<class TSelf, class... TInvokeArgs>
	inline ScopedDelegateHolder AddShared(TSelf* Self, void (TSelf::*func)(TInvokeArgs...)) requires
		requires { { Self->shared_from_this() }; }
	{
		return Add(CreateShared(Self, func));
	}

	inline ScopedDelegateHolder Add(DelegateInstance&& Instance)
	{
		std::unique_lock Scope(Lock);
		*Instance.Id = ++Id;
		ScopedDelegateHolder Handle(this, Instance.Id);
		Payload.emplace(*Instance.Id, std::move(Instance));
		return Handle;
	}

	virtual void Remove(DelegateHandle& Handle) override
	{
		std::unique_lock Scope(Lock);
		if (Handle.IsValid())
		{
			Payload.erase(Handle.Delegate.Id);
			Handle.Delegate.Reset();
		}
	}

	template<class T>
	inline auto operator +=(T&& InBody) requires
		std::invocable<T, TArgs...>
	{
		return AddLambda(std::forward<T>(InBody));
	}

	inline auto operator +=(DelegateInstance&& Instance)
	{
		return Add(std::move(Instance));
	}

	inline auto operator -=(DelegateHandle& handle)
	{
		return Remove(handle);
	}

private:
	template<class U>
	static auto ImplGetHolder(const std::shared_ptr<U>& InObject)
	{
		return [Self = std::weak_ptr(InObject)]
		{
			return !Self.expired();
		};
	}

	template<class U>
	static auto ImplGetHolder(U* InObject)
	{
		return nullptr;
	}
};
