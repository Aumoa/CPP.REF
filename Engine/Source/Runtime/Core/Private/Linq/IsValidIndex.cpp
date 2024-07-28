// Copyright 2020-2024 Aumoa.lib. All right reserved.

export module Core:IsValidIndex;

export import :Std;

export namespace Linq
{
	namespace details
	{
		struct is_valid_index_adaptor_closure
		{
			const size_t Index;

			inline constexpr is_valid_index_adaptor_closure(size_t InIndex) noexcept
				: Index(InIndex)
			{
			}

			template<std::ranges::sized_range R>
			inline constexpr auto operator ()(const R& InRange) const noexcept
			{
				return std::ranges::size(InRange) > Index;
			}
		};

		struct is_valid_index_adaptor
		{
			inline constexpr auto operator ()(size_t Index) const noexcept
			{
				return is_valid_index_adaptor_closure(Index);
			}

			template<std::ranges::input_range R>
			inline constexpr auto operator ()(const R& InRange, size_t Index) const noexcept
			{
				return operator ()(Index)(InRange);
			}
		};

		template<std::ranges::sized_range R>
		inline constexpr auto operator |(const R& InRange, const is_valid_index_adaptor_closure& Adaptor) noexcept
		{
			return Adaptor(InRange);
		}
	}

	inline namespace views
	{
		constexpr details::is_valid_index_adaptor IsValidIndex;
	}
}