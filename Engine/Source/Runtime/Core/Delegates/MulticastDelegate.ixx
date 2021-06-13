// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:MulticastDelegate;

import std.core;
import :PrimitiveTypes;

using namespace std;

export
{
	/// <summary>
	/// Represents a multicast delegate that is, a delegate that can have more than one element in its invocation list.
	/// </summary>
	/// <typeparam name=""></typeparam>
	template<class>
	class MulticastDelegate
	{
	};

	/// <summary>
	/// Represents a multicast delegate that is, a delegate that can have more than one element in its invocation list.
	/// </summary>
	/// <typeparam name="...TArgs"> The type of function arguments. </typeparam>
	template<class... TArgs>
	class MulticastDelegate<void(TArgs...)>
	{
		map<int64, function<void(TArgs...)>> _functions;
		atomic<int64> _id = 0;

	public:
		~MulticastDelegate() noexcept
		{
		}

		/// <summary>
		/// Invoke all functions.
		/// </summary>
		/// <param name="...args"> The function arguments. </param>
		void Invoke(TArgs&&... args)
		{
			for (auto it : _functions)
			{
				it.second(forward<TArgs>(args)...);
			}
		}

		/// <summary>
		/// Add raw function to multicast delegate.
		/// </summary>
		/// <param name="func"> The raw function. </param>
		/// <returns> Return valid function id if succeeded to bind, otherwise return -1. </returns>
		int64 AddRaw(function<void(TArgs...)> func)
		{
			int64 id = _id++;
			auto pair = _functions.emplace(id, move(func));
			return pair.second ? id : -1;
		}

		/// <summary>
		/// Remove binded function using delegate id.
		/// </summary>
		void RemoveRaw(int64 id)
		{
			_functions.erase(id);
		}

		int64 operator +=(function<void(TArgs...)> func)
		{
			return AddRaw(func);
		}
	};
}