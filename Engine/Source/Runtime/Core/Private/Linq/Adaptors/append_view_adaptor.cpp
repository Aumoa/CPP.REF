// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:append_view_adaptor;

export import :Std;
export import :append_view;

export namespace Linq::adaptors
{
	template<class T>
	struct append_view_adaptor_closure
	{
		T value;

		template<class U>
		constexpr append_view_adaptor_closure(U&& v) noexcept
			: value(std::forward<U>(v))
		{
		}

		template<std::ranges::input_range R>
		constexpr auto operator ()(R&& left_view) noexcept
		{
			return ranges::append_view(std::forward<R>(left_view), std::forward<T>(value));
		}
	};

	template<class T>
	append_view_adaptor_closure(T&&) -> append_view_adaptor_closure<T>;
	
	struct append_view_adaptor
	{
		template<class T>
		constexpr auto operator ()(T&& value) const noexcept
		{
			return append_view_adaptor_closure(std::forward<T>(value));
		}
	};

	template<std::ranges::input_range R, class T>
	constexpr auto operator |(R&& view, append_view_adaptor_closure<T>&& adaptor) noexcept
	{
		return adaptor(std::forward<R>(view));
	}
}