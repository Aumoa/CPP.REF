// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:any_adaptor;

export import :Std;

export namespace Linq::adaptors
{
	struct any_adaptor_closure
	{
		template<std::ranges::input_range R>
		constexpr auto operator ()(R&& view) const noexcept
		{
			if constexpr (std::ranges::sized_range<R>)
			{
				return std::ranges::size(std::forward<R>(view)) > 0;
			}
			else
			{
				return std::ranges::empty(std::forward<R>(view)) == false;
			}
		}
	};

	template<class T>
	struct any_adaptor_closure_lambda
	{
		T predicate;

		template<class U>
		constexpr any_adaptor_closure_lambda(U&& predicate) noexcept : predicate(std::forward<U>(predicate))
		{
		}

		template<std::ranges::input_range R>
		constexpr auto operator ()(R&& view) const noexcept
		{
			for (auto& v : view)
			{
				if ((bool)predicate(v))
				{
					return true;
				}
			}

			return false;
		}
	};

	template<class T>
	any_adaptor_closure_lambda(T&&) -> any_adaptor_closure_lambda<T>;

	struct any_adaptor
	{
		constexpr auto operator ()() const noexcept
		{
			return any_adaptor_closure();
		}

		template<class T>
		constexpr auto operator ()(T&& predicate) const noexcept
		{
			return any_adaptor_closure_lambda(std::forward<T>(predicate));
		}
	};

	template<std::ranges::input_range R>
	constexpr auto operator |(R&& view, any_adaptor_closure&& adaptor) noexcept
	{
		return adaptor(std::forward<R>(view));
	}

	template<std::ranges::input_range R, std::invocable<typename std::iterator_traits<std::ranges::iterator_t<R>>::value_type> T>
	constexpr auto operator |(R&& view, any_adaptor_closure_lambda<T>&& adaptor) noexcept
	{
		return adaptor(std::forward<R>(view));
	}
}