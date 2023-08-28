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
	};
}

template<class... TArgs>
using Func = typename details::function_args<TArgs...>::type;