// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "Threading/Spinlock.h"
#include <functional>
#include <vector>
#include <concepts>

namespace Ayla
{
	template<class... TArgs>
	class Action
	{
		using raw_t = void(TArgs...);
		using function_t = std::function<raw_t>;
		using vector_t = std::vector<function_t>;

	private:
		std::shared_ptr<vector_t> m_InvocationList;

	private:
		inline Action(std::shared_ptr<vector_t>&& invocationList)
			: m_InvocationList{ std::move(invocationList) }
		{
		}

	public:
		inline Action()
			: m_InvocationList{ std::make_shared<vector_t>() }
		{
		}

		template<class... UArgs>
		inline Action(UArgs&&... args)
			: m_InvocationList{ std::make_shared<vector_t>(vector_t{ function_t(std::forward<UArgs>(args)...) }) }
		{
		}

		inline Action(const Action& action)
			: m_InvocationList{ action.m_InvocationList }
		{
		}

		inline Action(Action&& action) noexcept
			: m_InvocationList{ std::move(action.m_InvocationList) }
		{
		}

		[[nodiscard]]
		inline bool IsBound() const
		{
			auto invocationList = m_InvocationList;
			return invocationList->size() > 0;
		}

		template<class... UArgs> requires std::invocable<function_t, UArgs...>
		inline void Invoke(UArgs&&... args) const
		{
			auto invocationList = m_InvocationList;
			for (auto& invocation : *invocationList)
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
			m_InvocationList = std::make_shared<vector_t>();
		}

		[[nodiscard]]
		inline operator bool() const noexcept
		{
			return IsBound();
		}

		inline Action& operator =(const Action& action)
		{
			m_InvocationList = action.m_InvocationList;
			return *this;
		}

		inline Action& operator =(Action&& action)
		{
			if (this == &action)
			{
				return *this;
			}

			m_InvocationList = std::move(action.m_InvocationList);
			return *this;
		}

		inline Action& operator =(std::nullptr_t) noexcept
		{
			Clear();
			return *this;
		}

		inline Action& operator +=(const Action& action)
		{
			auto selfInvocationList = m_InvocationList;
			auto addInvocationList = action.m_InvocationList;
			auto newInvocationList = std::make_shared<vector_t>();
			for (const auto& invocation : *selfInvocationList)
			{
				newInvocationList->emplace_back(invocation);
			}
			for (const auto& invocation : *addInvocationList)
			{
				newInvocationList->emplace_back(invocation);
			}
			
			m_InvocationList = newInvocationList;
			return *this;
		}

		inline Action& operator -=(const Action& action)
		{
			auto selfInvocationList = m_InvocationList;
			auto removeInvocationList = action.m_InvocationList;
			auto newInvocationList = std::make_shared<vector_t>(*selfInvocationList);

			for (const auto& toRemove : *removeInvocationList)
			{
				auto pred = [&](const function_t& f)
				{
					return f.target_type() == toRemove.target_type()
						&& f.template target<raw_t>() == toRemove.template target<raw_t>();
				};

				auto it = std::find_if(newInvocationList->begin(), newInvocationList->end(), pred);
				if (it != newInvocationList->end())
				{
					newInvocationList->erase(it);
				}
			}

			m_InvocationList = newInvocationList;
			return *this;
		}

		[[nodiscard]]
		inline constexpr bool operator ==(std::nullptr_t) const noexcept
		{
			return !IsBound();
		}

		[[nodiscard]]
		inline constexpr bool operator !=(std::nullptr_t) const noexcept
		{
			return IsBound();
		}

		template<class... UArgs> requires std::invocable<function_t, UArgs...>
		inline void operator ()(UArgs&&... args) const
		{
			Invoke(std::forward<UArgs>(args)...);
		}

		[[nodiscard]]
		static Action Combine(const Action& action1, const Action& action2)
		{
			Action result = std::make_shared<vector_t>(*action1.m_InvocationList);
			auto addInvocationList = action2.m_InvocationList;
			result.m_InvocationList.insert(addInvocationList->begin(), addInvocationList->end());
			return result;
		}

		[[nodiscard]]
		inline Action operator +(const Action& addAction) const
		{
			return Combine(*this, addAction);
		}

		[[nodiscard]]
		static Action Remove(const Action& action1, const Action& action2)
		{
			auto selfInvocationList = action1.m_InvocationList;
			auto removeInvocationList = action2.m_InvocationList;
			auto newInvocationList = std::make_shared<vector_t>(*selfInvocationList);

			for (const auto& toRemove : *removeInvocationList)
			{
				auto pred = [&](const function_t& f)
				{
					return f.target_type() == toRemove.target_type()
						&& f.template target<void>() == toRemove.template target<void>();
				};

				auto it = std::find_if(newInvocationList->begin(), newInvocationList->end(), pred);
				if (it != newInvocationList->end())
				{
					newInvocationList->erase(it);
				}
			}

			return newInvocationList;
		}

		[[nodiscard]]
		inline Action operator -(const Action& removeAction) const
		{
			return Remove(*this, removeAction);
		}
	};
}