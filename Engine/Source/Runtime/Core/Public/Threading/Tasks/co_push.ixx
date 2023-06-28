// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.Threading:co_push;

export import Core.Std;

export template<class T>
class co_push
{
	co_push() = delete;
	co_push(const co_push&) = delete;

private:
	T Item;

public:
	template<class... TArgs> requires
		std::constructible_from<T, TArgs...>
	co_push(TArgs&&... InArgs) noexcept(noexcept(T(std::forward<TArgs>(InArgs)...)))
		: Item(std::forward<TArgs>(InArgs)...)
	{
	}

	T Consume() &&
	{
		return std::move(Item);
	}
};

export template<class... TArgs> requires
	std::constructible_from<std::stop_token, TArgs...>
co_push(TArgs&&...) -> co_push<std::stop_token>;

export template<class... TArgs> requires
	std::constructible_from<std::function<bool()>, TArgs...>
co_push(TArgs&&...) -> co_push<std::function<bool()>>;