// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <functional>
#include <concepts>
#include "Threading/Spinlock.h"
#include "NullReferenceException.h"

namespace Ayla
{
	template<size_t Index, class T, class... TArgs>
	struct variant_index
	{
		using type = typename variant_index<Index - 1, TArgs...>::type;
	};

	template<class T, class... TArgs>
	struct variant_index<0, T, TArgs...>
	{
		using type = T;
	};

	template<size_t Index, class... TArgs>
	using variant_index_t = typename variant_index<Index, TArgs...>::type;

	template<class... TArgs>
	struct function_args
	{
	private:
		template<size_t... Index>
		static auto generate_signature(std::index_sequence<Index...>&&)
		{
			using return_t = variant_index_t<sizeof...(TArgs) - 1, TArgs...>;
			return std::function<return_t(
				variant_index_t<Index, TArgs...>...
			)>();
		}

	public:
		using type = decltype(generate_signature(std::make_index_sequence<sizeof...(TArgs) - 1>{}));

		template<size_t... Index>
		static auto function_reinterpret_cast(void(*func)(), std::index_sequence<Index...>&&)
		{
			using return_t = variant_index_t<sizeof...(TArgs) - 1, TArgs...>;
			return reinterpret_cast<return_t(*)(variant_index_t<Index, TArgs...>...)>(func);
		}
	};

	template<class... TArgs>
	class Func
	{
		using function_t = ::Ayla::function_args<TArgs...>::type;

	private:
		mutable Spinlock m_Lock;
		std::vector<function_t> m_InvocationList;

	public:
		inline Func() noexcept
		{
		}

		template<class... UArgs>
		inline Func(UArgs&&... args) requires std::constructible_from<function_t, UArgs...>
			: m_InvocationList{ { function_t(std::forward<UArgs>(args))... } }
		{
		}

		inline Func(const Func& func)
		{
			auto lock = std::unique_lock(func.m_Lock);
			m_InvocationList = func.m_InvocationList;
		}

		inline Func(Func&& func) noexcept
		{
			auto lock = std::unique_lock(func.m_Lock);
			m_InvocationList = std::move(func.m_InvocationList);
		}

		[[nodiscard]] bool IsBound() const
		{
			return this->operator bool();
		}

		template<class... UArgs> requires std::invocable<function_t, UArgs...>
		inline auto Invoke(UArgs&&... args) const
		{
			auto lock = std::unique_lock(m_Lock);
			auto invocationListCopy = m_InvocationList;
			lock.unlock();

			if (invocationListCopy.size() > 0)
			{
				for (size_t i = 0; i < invocationListCopy.size() - 1; ++i)
				{
					invocationListCopy[i](std::forward<UArgs>(args)...);
				}

				return invocationListCopy.back(std::forward<UArgs>(args)...);
			}
			else
			{
				throw new NullReferenceException();
			}
		}

		template<class... UArgs> requires std::invocable<function_t, UArgs...>
		inline auto InvokeIfBound(UArgs&&... args) const
		{
			if constexpr (std::same_as<std::invoke_result_t<function_t>, void>)
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
			else
			{
				if (IsBound())
				{
					return std::optional(Invoke(std::forward<UArgs>(args)...));
				}
				else
				{
					return std::optional<std::invoke_result_t<function_t>>{};
				}
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

		inline Func& operator =(const Func& func)
		{
			std::unique_lock<Spinlock> lock1, lock2;
			bool same = this == &func;

			lock1 = std::unique_lock(m_Lock);
			if (same == false)
			{
				lock2 = std::unique_lock(func.m_Lock);
			}

			m_InvocationList = func.m_InvocationList;
			return *this;
		}

		inline Func& operator =(Func&& func)
		{
			std::unique_lock<Spinlock> lock1, lock2;
			bool same = this == &func;

			lock1 = std::unique_lock(m_Lock);
			if (same == false)
			{
				lock2 = std::unique_lock(func.m_Lock);
			}

			m_InvocationList = std::move(func.m_InvocationList);
			return *this;
		}

		inline Func& operator =(std::nullptr_t) noexcept
		{
			auto lock = std::unique_lock(m_Lock);
			m_InvocationList.clear();
		}

		inline Func& operator +=(const Func& func)
		{
			std::unique_lock<Spinlock> lock1, lock2;
			bool same = this == &func;

			lock1 = std::unique_lock(m_Lock);
			if (same == false)
			{
				lock2 = std::unique_lock(func.m_Lock);
			}

			m_InvocationList.insert(m_InvocationList.end(), func.m_InvocationList.begin(), func.m_InvocationList.end());
			return *this;
		}

		inline Func& operator -=(const Func& func)
		{
			std::unique_lock<Spinlock> lock1, lock2;
			bool same = this == &func;

			lock1 = std::unique_lock(m_Lock);
			if (same)
			{
				m_InvocationList.clear();
				return *this;
			}
			else
			{
				lock2 = std::unique_lock(func.m_Lock);
			}

			auto it = std::search(m_InvocationList.rbegin(), m_InvocationList.rend(), func.m_InvocationList.rbegin(), func.m_InvocationList.rend());
			if (it == m_InvocationList.rend())
			{
				return *this;
			}

			m_InvocationList.erase(it, it + func.m_InvocationList.size());
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
		inline auto operator ()(UArgs&&... args) const
		{
			return Invoke(std::forward<UArgs>(args)...);
		}

		[[nodiscard]]
		static Func Combine(const Func& action1, const Func& action2)
		{
			std::unique_lock<Spinlock> lock1, lock2;
			bool same = &action1 == &action2;

			lock1 = std::unique_lock(action1.m_Lock);
			if (same == false)
			{
				lock2 = std::unique_lock(action2.m_Lock);
			}

			Func result;
			result.m_InvocationList = action1.m_InvocationList;
			result.m_InvocationList.insert(result.m_InvocationList.begin(), action2.m_InvocationList.begin(), action2.m_InvocationList.end());
			return result;
		}

		[[nodiscard]]
		static Func Remove(const Func& action1, const Func& action2)
		{
			bool same = &action1 == &action2;
			if (same)
			{
				return Func{};
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
		static Func RemoveAll(const Func& action1, const Func& action2)
		{
			bool same = &action1 == &action2;
			if (same)
			{
				return Func{};
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

			Func result;
			result.m_InvocationList = copiedInvocationList;
			return result;
		}

		inline static Func FromAnonymous(void(*AnnPtr)())
		{
			return Func(function_args<TArgs...>::function_reinterpret_cast(AnnPtr, std::make_index_sequence<sizeof...(TArgs) - 1>{}));
		}
	};
}
