// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:MulticastEvent;

import std.core;
import :MulticastDelegate;

using namespace std;

export
{
	/// <summary>
	/// Represents a multicast event that is, a event that can have more than one element in its invocation list.
	/// </summary>
	/// <typeparam name="...TArgs"> The type of function arguments. </typeparam>
	template<class T, class F>
	class MulticastEvent : public MulticastDelegate<F>
	{
		friend T;

	private:
		using MulticastDelegate<F>::Invoke;
	};
}