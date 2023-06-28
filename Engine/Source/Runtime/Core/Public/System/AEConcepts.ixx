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

export template<class... T>
struct Variadic
{
	static constexpr const size_t Count = sizeof...(T);
};

export template<size_t Index, class T, class... TArgs>
struct GetVariadic
{
	using Type = typename GetVariadic<Index - 1, TArgs...>::Type;
};

export template<size_t Index, class... TArgs>
struct GetVariadic<Index, Variadic<TArgs...>> : public GetVariadic<Index, TArgs...>
{
};

export template<class T, class... TArgs>
struct GetVariadic<0, T, TArgs...>
{
	using Type = T;
};

export template<class... TArgs>
struct GetVariadic<0, Variadic<TArgs...>> : public GetVariadic<0, TArgs...>
{
};

export template<size_t Index, class... TArgs>
using GetVariadic_t = typename GetVariadic<Index, TArgs...>::Type;