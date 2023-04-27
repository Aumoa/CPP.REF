// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:MulticastEvent;

export import :MulticastDelegate;

/// <summary>
/// Represents a multicast event that is, a event that can have more than one element in its invocation list.
/// </summary>
/// <typeparam name="...TArgs"> The type of function arguments. </typeparam>
export template<class T, class F>
class MulticastEvent : public MulticastDelegate<F>
{
	friend T;

private:
	using MulticastDelegate<F>::Broadcast;
};