// Copyright 2020-2021 Aumoa.lib. All right reserved.

export module SC.Runtime.Core:MulticastDelegate;

import std.core;

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
	/// Represents a delegate handle to unlink multicast delegate. The destructor will be unlink target.
	/// </summary>
	class DelegateHandle
	{
		template<class>
		friend class MulticastDelegate;

		function<void()> _remove;
		DelegateHandle();

	public:
		~DelegateHandle();

		/// <summary>
		/// Unlink target multicast delegate.
		/// </summary>
		void Unlink() noexcept;

		/// <summary>
		/// Get validation state that represents link state.
		/// </summary>
		/// <returns> The link state. </returns>
		bool IsValid() const;
	};

	/// <summary>
	/// Represents a multicast delegate that is, a delegate that can have more than one element in its invocation list.
	/// </summary>
	/// <typeparam name="...TArgs"> The type of function arguments. </typeparam>
	template<class... TArgs>
	class MulticastDelegate<void(TArgs...)>
	{
		set<function<void(TArgs...)>> _functions;
		set<DelegateHandle*> _handles;

	public:
		~MulticastDelegate() noexcept
		{
			for (auto it = _handles.begin(); _handles.size() != 0; it = _handles.begin())
			{
				(*it)->Unlink();
				_handles.erase(it);
			}
		}

		/// <summary>
		/// Invoke all functions.
		/// </summary>
		/// <param name="...args"> The function arguments. </param>
		void Invoke(TArgs&&... args)
		{
			for (auto it : _functions)
			{
				it(forward<TArgs>(args)...);
			}
		}

		/// <summary>
		/// Add raw function to multicast delegate.
		/// </summary>
		/// <param name="func"> The raw function. </param>
		/// <returns> Valid delegate handle if binding succeeded, otherwise invalid handle. See <see cref="DelegateHandle::IsValid"/> function to check validation. </returns>
		DelegateHandle AddRaw(function<void(TArgs...)> func)
		{
			auto pair = _functions.emplace(move(func));
			if (!pair.second)
			{
				// Return invalid handle.
				return DelegateHandle();
			}
			
			DelegateHandle handle;
			auto pair_handle = _handles.emplace(&handle);
			handle._remove = [this, it = pair.first, handle_it = pair_handle.first]()
			{
				_functions.erase(it);
				_handles.erase(handle_it);
			};

			return handle;
		}

		DelegateHandle operator +=(function<void(TArgs...)> func)
		{
			return AddRaw(func);
		}
	};
}