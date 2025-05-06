// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <functional>
#include "Threading/Spinlock.h"

namespace Ayla
{
	template<class... TArgs>
	class Action
	{
		using function_t = std::function<void(TArgs...)>;

	private:
		mutable Spinlock m_Lock;
		std::vector<function_t> m_InvocationList;

	public:
		inline Action() noexcept
		{
		}

		template<class... UArgs>
		inline Action(UArgs&&... args) requires std::constructible_from<function_t, UArgs...>
			: m_InvocationList{ { function_t(std::forward<UArgs>(args))...} }
		{
		}

		inline Action(const Action& action)
		{
			auto lock = std::unique_lock(action.m_Lock);
			m_InvocationList = action.m_InvocationList;
		}

		inline Action(Action&& action) noexcept
		{
			auto lock = std::unique_lock(action.m_Lock);
			m_InvocationList = std::move(action.m_InvocationList);
		}

		[[nodiscard]]
		inline bool IsBound() const
		{
			auto lock = std::unique_lock(m_Lock);
			return m_InvocationList.size() > 0;
		}

		template<class... UArgs> requires std::invocable<function_t, UArgs...>
		inline void Invoke(UArgs&&... args) const
		{
			auto lock = std::unique_lock(m_Lock);
			auto invocationListCopy = m_InvocationList;
			lock.unlock();

			for (auto& invocation : invocationListCopy)
			{
				invocation(std::forward<UArgs>(args)...);
			}
		}

		template<class... UArgs> requires std::invocable<function_t, UArgs...>
		inline bool InvokeIfBound(UArgs&&... args) const
		{
			if (IsBound())
			{
				Invoke(std::forward<UArgs>(args)...);
				return true;
			}
			else
			{
				return false;
			}
		}

		inline void Clear() noexcept
		{
			auto lock = std::unique_lock(m_Lock);
			m_InvocationList.clear();
		}

		[[nodiscard]]
		inline operator bool() const noexcept
		{
			auto lock = std::unique_lock(m_Lock);
			return m_InvocationList.size() > 0;
		}

		inline Action& operator =(const Action& action)
		{
			std::unique_lock<Spinlock> lock1, lock2;
			bool same = this == &action;

			lock1 = std::unique_lock(m_Lock);
			if (same == false)
			{
				lock2 = std::unique_lock(action.m_Lock);
			}

			m_InvocationList = action.m_InvocationList;
			return *this;
		}

		inline Action& operator =(Action&& action)
		{
			std::unique_lock<Spinlock> lock1, lock2;
			bool same = this == &action;

			lock1 = std::unique_lock(m_Lock);
			if (same == false)
			{
				lock2 = std::unique_lock(action.m_Lock);
			}

			m_InvocationList = std::move(action.m_InvocationList);
			return *this;
		}

		inline Action& operator =(std::nullptr_t) noexcept
		{
			auto lock = std::unique_lock(m_Lock);
			m_InvocationList.clear();
		}

		inline Action& operator +=(const Action& action)
		{
			std::unique_lock<Spinlock> lock1, lock2;
			bool same = this == &action;

			lock1 = std::unique_lock(m_Lock);
			if (same == false)
			{
				lock2 = std::unique_lock(action.m_Lock);
			}

			m_InvocationList.insert(m_InvocationList.end(), action.m_InvocationList.begin(), action.m_InvocationList.end());
			return *this;
		}

		inline Action& operator -=(const Action& action)
		{
			std::unique_lock<Spinlock> lock1, lock2;
			bool same = this == &action;

			lock1 = std::unique_lock(m_Lock);
			if (same)
			{
				m_InvocationList.clear();
				return *this;
			}
			else
			{
				lock2 = std::unique_lock(action.m_Lock);
			}

			auto it = std::search(m_InvocationList.rbegin(), m_InvocationList.rend(), action.m_InvocationList.rbegin(), action.m_InvocationList.rend());
			if (it == m_InvocationList.rend())
			{
				return *this;
			}

			m_InvocationList.erase(it, it + action.m_InvocationList.size());
			return *this;
		}

		[[nodiscard]]
		inline constexpr bool operator ==(std::nullptr_t) const noexcept
		{
			auto lock = std::unique_lock(m_Lock);
			return m_InvocationList.size() == 0;
		}

		[[nodiscard]]
		inline constexpr bool operator !=(std::nullptr_t) const noexcept
		{
			auto lock = std::unique_lock(m_Lock);
			return m_InvocationList.size() > 0;
		}

		template<class... UArgs> requires std::invocable<function_t, UArgs...>
		inline void operator ()(UArgs&&... args) const
		{
			Invoke(std::forward<UArgs>(args)...);
		}

		[[nodiscard]]
		static Action Combine(const Action& action1, const Action& action2)
		{
			std::unique_lock<Spinlock> lock1, lock2;
			bool same = &action1 == &action2;

			lock1 = std::unique_lock(action1.m_Lock);
			if (same == false)
			{
				lock2 = std::unique_lock(action2.m_Lock);
			}

			Action result;
			result.m_InvocationList = action1.m_InvocationList;
			result.m_InvocationList.insert(result.m_InvocationList.begin(), action2.m_InvocationList.begin(), action2.m_InvocationList.end());
			return result;
		}

		[[nodiscard]]
		static Action Remove(const Action& action1, const Action& action2)
		{
			bool same = &action1 == &action2;
			if (same)
			{
				return Action{};
			}

			auto lock1 = std::unique_lock(action1.m_Lock);
			auto lock2 = std::unique_lock(action2.m_Lock);

			auto it = std::search(action1.m_InvocationList.rbegin(), action1.m_InvocationList.rend(), action2.m_InvocationList.rbegin(), action2.m_InvocationList.rend());
			if (it == action1.m_InvocationList.rend())
			{
				return *this;
			}

			std::vector<function_t> copiedInvocationList = action1.m_InvocationList;
			copiedInvocationList.erase(it, it + action2.m_InvocationList.size());
			return *this;
		}

		[[nodiscard]]
		static Action RemoveAll(const Action& action1, const Action& action2)
		{
			bool same = &action1 == &action2;
			if (same)
			{
				return Action{};
			}

			auto lock1 = std::unique_lock(action1.m_Lock);
			auto lock2 = std::unique_lock(action2.m_Lock);
			std::vector<function_t> copiedInvocationList = action1.m_InvocationList;

			while (true)
			{
				auto it = std::search(copiedInvocationList.rbegin(), copiedInvocationList.rend(), action2.m_InvocationList.rbegin(), action2.m_InvocationList.rend());
				if (it == copiedInvocationList.rend())
				{
					break;
				}

				copiedInvocationList.erase(it, it + action2.m_InvocationList.size());
			}

			Action result;
			result.m_InvocationList = copiedInvocationList;
			return result;
		}
	};
}