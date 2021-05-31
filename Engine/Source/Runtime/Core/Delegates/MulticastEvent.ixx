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
	template<class T, class... TArgs>
	class MulticastEvent : public MulticastDelegate<TArgs...>
	{
		friend T;

	private:
		/// <summary>
		/// Invoke all functions.
		/// </summary>
		/// <param name="...args"> The function arguments. </param>
		void Invoke(TArgs&&... args)
		{
			MulticastDelegate<TArgs...>::Invoke(forward<TArgs>(args)...);
		}
	};
}