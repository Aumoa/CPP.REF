// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:DelegateHandle;

export import :IntegralTypes;
export import :ScopedDelegateHolder;

export class DelegateHandle
{
	template<class>
	friend class MulticastDelegate;

private:
	ScopedDelegateHolder Delegate;

public:
	DelegateHandle() = default;
	DelegateHandle(const DelegateHandle&) = default;
	DelegateHandle(DelegateHandle&&) = default;

	DelegateHandle(const ScopedDelegateHolder& Delegate)
		: Delegate(Delegate)
	{
	}

	DelegateHandle(ScopedDelegateHolder&& Delegate)
		: Delegate(std::move(Delegate))
	{
	}

	bool IsValid() const
	{
		return Delegate.IsValid();
	}

	void Reset()
	{
		if (IsValid())
		{
			Delegate->Remove(*this);
			Delegate.Reset();
		}
	}

	auto operator <=>(const DelegateHandle& Rhs) const { return Delegate <=> Rhs.Delegate; }
	bool operator ==(const DelegateHandle& Rhs) const { return Delegate == Rhs.Delegate; }

	DelegateHandle& operator =(const DelegateHandle&) = default;
	DelegateHandle& operator =(DelegateHandle&&) = default;
};