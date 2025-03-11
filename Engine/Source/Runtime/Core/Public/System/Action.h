// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include <functional>

template<class... TArgs>
class Action : private std::function<void(TArgs...)>
{
	using function_t = std::function<void(TArgs...)>;

public:
	inline Action()
	{
	}

	template<class... UArgs>
	inline Action(UArgs&&... InArgs) requires std::constructible_from<function_t, UArgs...>
		: function_t(std::forward<UArgs>(InArgs)...)
	{
	}

	[[nodiscard]] bool IsBound() const
	{
		return this->operator bool();
	}

	template<class... UArgs> requires std::invocable<function_t, UArgs...>
	inline void Invoke(UArgs&&... InArgs)
	{
		this->operator ()(std::forward<UArgs>(InArgs)...);
	}

	template<class... UArgs> requires std::invocable<function_t, UArgs...>
	inline void InvokeIfBound(UArgs&&... InArgs)
	{
		if (IsBound())
		{
			Invoke(std::forward<UArgs>(InArgs)...);
		}
	}

	using function_t::operator bool;
	using function_t::operator ();
};