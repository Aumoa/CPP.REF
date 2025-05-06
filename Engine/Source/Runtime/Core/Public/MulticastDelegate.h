// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Delegate.h"
#include "Referencer.h"
#include "DelegateHandle.h"
#include "Action.h"
#include "AssertionMacros.h"
#include "Threading/Spinlock.h"
#include "Linq/Any.h"
#include <unordered_map>

namespace Ayla
{
	template<class T>
	class MulticastDelegate
	{
		static_assert(std::same_as<T, std::function<void>>, "invalid template argument.");
	};

	template<class... TArgs>
	class MulticastDelegate<void(TArgs...)> : public Action<TArgs...>
	{
	public:
		MulticastDelegate() noexcept = default;
		MulticastDelegate(const MulticastDelegate&) = default;
		MulticastDelegate(MulticastDelegate&&) noexcept = default;

		MulticastDelegate& operator =(const MulticastDelegate&) = default;
		MulticastDelegate& operator =(MulticastDelegate&&) = default;
	};

#define DECLARE_MULTICAST_DELEGATE(DelegateName, ...) using DelegateName = ::Ayla::MulticastDelegate<void(__VA_ARGS__)>
}