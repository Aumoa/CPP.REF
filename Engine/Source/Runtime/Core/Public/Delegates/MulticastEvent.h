// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "MulticastDelegate.h"

/// <summary>
/// Represents a multicast event that is, a event that can have more than one element in its invocation list.
/// </summary>
/// <typeparam name="...TArgs"> The type of function arguments. </typeparam>
template<class T, class F>
class MulticastEvent : public MulticastDelegate<F>
{
	friend T;

private:
	using MulticastDelegate<F>::Broadcast;
};

#define DECLARE_MULTICAST_EVENT(EventName, ...)					\
	using EventName = ::MulticastEvent<This, void(__VA_ARGS__)>;
