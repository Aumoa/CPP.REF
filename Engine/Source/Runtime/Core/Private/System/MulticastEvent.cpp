// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:MulticastEvent;

export import :MulticastDelegate;

export template<class T, class F>
class MulticastEvent : public MulticastDelegate<F>
{
	friend T;

private:
	using MulticastDelegate<F>::Broadcast;
};