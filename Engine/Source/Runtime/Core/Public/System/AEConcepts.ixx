// Copyright 2020-2022 Aumoa.lib. All right reserved.

export module Core.System:AEConcepts;

export import Core.Std;

export template<class TBase, class... TArgs>
concept CompatibleVariadic = requires (TArgs&&... Vals)
{
	{ std::initializer_list<TBase>{ TBase(Vals)... } };
};

export template<class R>
concept FixedSizeRange = requires (R&& Val)
{
	{ std::size(Val) } -> std::convertible_to<size_t>;
};