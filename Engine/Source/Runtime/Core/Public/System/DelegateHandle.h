// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/IntegralTypes.h"

template<class T>
class MulticastDelegate;

class DelegateHandle
{
	template<class T>
	friend class MulticastDelegate;

private:
	int64 InstanceId;
	int64 Id;

public:
	constexpr DelegateHandle() noexcept
		: InstanceId(0)
		, Id(0)
	{
	}

	constexpr DelegateHandle(int64 InstanceId, int64 Id) noexcept
		: InstanceId(InstanceId)
		, Id(Id)
	{
	}

	constexpr DelegateHandle(const DelegateHandle&) noexcept = default;
	constexpr DelegateHandle(DelegateHandle&&) noexcept = default;

	constexpr bool IsValid() const noexcept
	{
		return InstanceId != 0 && Id != 0;
	}

	constexpr void Reset() noexcept
	{
		InstanceId = 0;
		Id = 0;
	}

	constexpr auto operator <=>(const DelegateHandle& Rhs) const noexcept { return Id <=> Rhs.Id; }
	constexpr bool operator ==(const DelegateHandle& Rhs) const noexcept { return Id == Rhs.Id; }

	constexpr DelegateHandle& operator =(const DelegateHandle&) noexcept = default;
	constexpr DelegateHandle& operator =(DelegateHandle&&) noexcept = default;
};