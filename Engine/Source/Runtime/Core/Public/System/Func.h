// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include <functional>
#include <concepts>

namespace details
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
}

template<class... TArgs>
class Func : private details::function_args<TArgs...>::type
{
	using function_t = details::function_args<TArgs...>::type;

public:
	inline Func()
	{
	}

	template<class... UArgs>
	inline Func(UArgs&&... InArgs) requires std::constructible_from<function_t, UArgs...>
		: function_t(std::forward<UArgs>(InArgs)...)
	{
	}

	[[nodiscard]] bool IsBound() const
	{
		return this->operator bool();
	}

	template<class... UArgs> requires std::invocable<function_t, UArgs...>
	inline auto Invoke(UArgs&&... InArgs)
	{
		return this->operator ()(std::forward<UArgs>(InArgs)...);
	}

	using function_t::operator bool;
	using function_t::operator ();

	inline static Func FromAnonymous(void(*AnnPtr)())
	{
		return Func(details::function_args<TArgs...>::function_reinterpret_cast(AnnPtr, std::make_index_sequence<sizeof...(TArgs) - 1>{}));
	}
};