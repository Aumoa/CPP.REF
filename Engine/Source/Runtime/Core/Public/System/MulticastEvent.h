// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "System/MulticastDelegate.h"

template<class T, class F>
class MulticastEvent : public MulticastDelegate<F>
{
	friend T;

private:
	using MulticastDelegate<F>::Broadcast;
};

#define DECLARE_MULTICAST_EVENT(EventName, ...) using EventName = MulticastEvent<void(__VA_ARGS__)>