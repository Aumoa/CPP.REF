// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <functional>
#include <concepts>
#include <vector>
#include "Threading/Spinlock.h"
#include "NullReferenceException.h"

namespace Ayla
{
	namespace Func_Internal
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
		public:
			using return_t = variant_index_t<sizeof...(TArgs) - 1, TArgs...>;

		private:
			template<size_t... Index>
			static auto generate_signature_raw(std::index_sequence<Index...>&&)
			{
				return (return_t(*)(variant_index_t<Index, TArgs...>...))nullptr;
			}

			template<size_t... Index>
			static auto generate_signature_func(std::index_sequence<Index...>&&)
			{
				return (std::function<return_t(variant_index_t<Index, TArgs...>...)>)nullptr;
			}

		public:
			using sig_raw = decltype(generate_signature_raw(std::make_index_sequence<sizeof...(TArgs) - 1>{}));
			using sig_func = decltype(generate_signature_func(std::make_index_sequence<sizeof...(TArgs) - 1>{}));
			

			template<size_t... Index>
			static auto function_reinterpret_cast(void(*func)(), std::index_sequence<Index...>&&)
			{
				using return_t = variant_index_t<sizeof...(TArgs) - 1, TArgs...>;
				return reinterpret_cast<return_t(*)(variant_index_t<Index, TArgs...>...)>(func);
			}
		};
	}

	template<class... TArgs>
	class Func
	{
		using raw_t = ::Ayla::Func_Internal::function_args<TArgs...>::sig_raw;
		using function_t = ::Ayla::Func_Internal::function_args<TArgs...>::sig_func;
		using vector_t = std::vector<function_t>;
		using result_t = ::Ayla::Func_Internal::function_args<TArgs...>::return_t;
		static_assert(std::same_as<result_t, void> == false);

	private:
		std::shared_ptr<vector_t> m_InvocationList;

	private:
		inline Func(std::shared_ptr<vector_t>&& invocationList)
			: m_InvocationList{ std::move(invocationList) }
		{
		}

	public:
		inline Func()
			: m_InvocationList{ std::make_shared<vector_t>() }
		{
		}

		template<class... UArgs>
		inline Func(UArgs&&... args) requires std::constructible_from<function_t, UArgs...>
			: m_InvocationList{ std::make_shared<vector_t>(vector_t{ function_t(std::forward<UArgs>(args)...) }) }
		{
		}

		inline Func(const Func& func)
			: m_InvocationList{ func.m_InvocationList }
		{
		}

		inline Func(Func&& func) noexcept
			: m_InvocationList{ std::move(func.m_InvocationList) }
		{
		}

		[[nodiscard]]
		inline bool IsBound() const
		{
			auto invocationList = m_InvocationList;
			return invocationList->size() > 0;
		}

		template<class... UArgs> requires std::invocable<function_t, UArgs...>
		inline result_t Invoke(UArgs&&... args) const
		{
			auto invocationList = m_InvocationList;
			size_t count = invocationList->size();
			for (size_t i = 0; i < count; ++i)
			{
				auto& invocation = invocationList->at(i);
				if (i == count - 1)
				{
					return invocation(std::forward<UArgs>(args)...);
				}
				else
				{
					invocation(std::forward<UArgs>(args)...);
				}
			}

			throw new NullReferenceException();
		}

		template<class... UArgs> requires std::invocable<function_t, UArgs...>
		inline std::optional<result_t> InvokeIfBound(UArgs&&... args) const
		{
			auto invocationList = m_InvocationList;
			size_t count = invocationList->size();
			for (size_t i = 0; i < count; ++i)
			{
				auto& invocation = invocationList->at(i);
				if (i == count - 1)
				{
					return std::optional<result_t>(invocation(std::forward<UArgs>(args)...));
				}
				else
				{
					invocation(std::forward<UArgs>(args)...);
				}
			}

			return std::optional<result_t>();
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

		inline Func& operator =(const Func& func)
		{
			m_InvocationList = func.m_InvocationList;
			return *this;
		}

		inline Func& operator =(Func&& func)
		{
			if (this == &func)
			{
				return *this;
			}

			m_InvocationList = std::move(func.m_InvocationList);
			return *this;
		}

		inline Func& operator =(std::nullptr_t) noexcept
		{
			Clear();
			return *this;
		}

		inline Func& operator +=(const Func& func)
		{
			auto selfInvocationList = m_InvocationList;
			auto addInvocationList = func.m_InvocationList;
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

		inline Func& operator -=(const Func& func)
		{
			auto selfInvocationList = m_InvocationList;
			auto removeInvocationList = func.m_InvocationList;
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
		inline auto operator ()(UArgs&&... args) const
		{
			return Invoke(std::forward<UArgs>(args)...);
		}

		[[nodiscard]]
		static Func Combine(const Func& func1, const Func& func2)
		{
			Action result = std::make_shared<vector_t>(*func1.m_InvocationList);
			auto addInvocationList = func2.m_InvocationList;
			result.m_InvocationList.insert(addInvocationList->begin(), addInvocationList->end());
			return result;
		}

		[[nodiscard]]
		inline Func operator +(const Func& addFunc) const
		{
			return Combine(*this, addFunc);
		}

		[[nodiscard]]
		static Func Remove(const Func& func1, const Func& func2)
		{
			auto selfInvocationList = func1.m_InvocationList;
			auto removeInvocationList = func2.m_InvocationList;
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
		inline Func operator -(const Func& removeFunc) const
		{
			return Remove(*this, removeFunc);
		}

		inline static Func FromAnonymous(void(*AnnPtr)())
		{
			return Func(Func_Internal::function_args<TArgs...>::function_reinterpret_cast(AnnPtr, std::make_index_sequence<sizeof...(TArgs) - 1>{}));
		}
	};
}
