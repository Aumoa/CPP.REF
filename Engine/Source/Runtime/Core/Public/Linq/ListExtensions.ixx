// Copyright 2020-2023 Aumoa.lib. All right reserved.

export module Core.Linq:ListExtensions;

export import Core.Std;

export namespace Linq
{
	template<std::ranges::input_range R>
	constexpr bool IsValidIndex(R&& Range, size_t Index) noexcept requires std::ranges::sized_range<R>
	{
		size_t Count = std::ranges::size(std::forward<R>(Range));
		return Index < Count;
	}
}