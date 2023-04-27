// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:ScopedDelegateHolder;

export import :IntegralTypes;
export import :IMulticastDelegate;
export import <memory>;

export class DelegateHandle;
export template<class> class MulticastDelegate;

export class ScopedDelegateHolder
{
	friend DelegateHandle;
	template<class>
	friend class MulticastDelegate;

	IMulticastDelegate* DelegatePtr = nullptr;
	std::weak_ptr<int64> IdPtr;
	int64 Id;

public:
	ScopedDelegateHolder() = default;
	ScopedDelegateHolder(const ScopedDelegateHolder&) = default;
	ScopedDelegateHolder(ScopedDelegateHolder&&) = default;

	ScopedDelegateHolder(IMulticastDelegate* DelegatePtr, std::shared_ptr<int64> Id)
		: DelegatePtr(DelegatePtr)
		, IdPtr(Id)
		, Id(*Id)
	{
	}

	bool IsValid() const
	{
		if (IdPtr.expired())
		{
			return false;
		}

		return DelegatePtr && Id != 0;
	}

	void Reset()
	{
		DelegatePtr = nullptr;
		IdPtr.reset();
		Id = 0;
	}

	IMulticastDelegate* operator ->() const
	{
		return DelegatePtr;
	}

	auto operator <=>(const ScopedDelegateHolder& Rhs) const { return Id <=> Rhs.Id; }
	bool operator ==(const ScopedDelegateHolder& Rhs) const { return Id == Rhs.Id; }

	ScopedDelegateHolder& operator =(const ScopedDelegateHolder&) = default;
	ScopedDelegateHolder& operator =(ScopedDelegateHolder&&) = default;
};