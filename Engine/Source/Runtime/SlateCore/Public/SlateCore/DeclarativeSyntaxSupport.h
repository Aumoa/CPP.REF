// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreConcepts.h"

template<class T, class TDesiredClass>
struct DeclarativeInheritanceIfImplements
{
private:
	template<class U> requires requires { typename U::template DeclarativeAttr<>; }
	static auto Impl(int32) -> decltype(std::declval<typename U::template DeclarativeAttr<TDesiredClass>>());

	struct E
	{
	};
	template<class U> requires (!requires { typename U::template DeclarativeAttr<>; })
	static auto Impl(int16) -> E;

public:
	using Type = std::remove_reference_t<decltype(Impl<T>(0))>;
};

template<class TVoid, class TImpl>
struct DeclarativeVoidTypeImpl
{
	using Type = TVoid;
};

template<class TImpl>
struct DeclarativeVoidTypeImpl<void, TImpl>
{
	using Type = TImpl;
};

template<class U>
concept DeclarativeAttrShouldBeCast = requires
{
	{ std::declval<typename U::DeclarativeAttrVoid>() };
};

#define BEGIN_SLATE_ATTRIBUTE																			\
template<class TThis = void>																			\
struct DeclarativeAttr : public DeclarativeInheritanceIfImplements<Super, DeclarativeAttr<>>::Type		\
{																										\
	using This = typename DeclarativeVoidTypeImpl<TThis, DeclarativeAttr>::Type;						\
																										\
	template<DeclarativeAttrShouldBeCast U = Super>														\
	operator typename U::DeclarativeAttrVoid&& () &														\
	{																									\
		return std::move(reinterpret_cast<typename U::DeclarativeAttrVoid&>(*this));					\
	}

#define END_SLATE_ATTRIBUTE								\
};														\
using DeclarativeAttrVoid = DeclarativeAttr<void>;

#define DECLARE_SLATE_ATTRIBUTE(Type, Var, ...)								\
Type _ ## Var __VA_OPT__(=) __VA_ARGS__;									\
This&& Var(const Type& value) &&											\
{																			\
	_ ## Var = value;														\
	return std::move(*static_cast<This*>(this));							\
}																			\
template<class... TArgs> requires std::constructible_from<Type, TArgs...>	\
This&& Var(TArgs&&... args) &&												\
{																			\
	_ ## Var = { std::forward<TArgs>(args)... };							\
	return std::move(*static_cast<This*>(this));							\
}																			\
																			\
This& Var(const Type& value) &												\
{																			\
	_ ## Var = value;														\
	return *static_cast<This*>(this);										\
}																			\
template<class... TArgs> requires std::constructible_from<Type, TArgs...>	\
This& Var(TArgs&&... args) &												\
{																			\
	_ ## Var = { std::forward<TArgs>(args)... };							\
	return *static_cast<This*>(this);										\
}

#define DECLARE_SLATE_CONTENT(Type)											\
SWidget* _Content = nullptr;												\
This&& operator [](SWidget* value) &&										\
{																			\
	_Content = value;														\
	return std::move(*static_cast<This*>(this));							\
}

#define DECLARE_SLATE_CONSTRUCTOR() void Construct(DeclarativeAttr<>&&)
#define DEFINE_SLATE_CONSTRUCTOR(SlateClass, AttrVar) void SlateClass::Construct(DeclarativeAttr<>&& AttrVar)
#define INVOKE_SLATE_CONSTRUCTOR_SUPER(AttrVar) Super::Construct(AttrVar)

#define DECLARE_SLATE_SLOT_SUPPORTS(SlotClass)				\
std::vector<SlotClass> Slots;								\
This&& operator +(SlotClass&& instanceSlot) &&				\
{															\
	Slots.emplace_back(std::move(instanceSlot));			\
	return std::move(*static_cast<This*>(this));			\
}

#define SNew(SlateClass) (gcnew SlateClass()) << SlateClass::DeclarativeAttr<>()
#define SAssignNew(Var, SlateClass) (Var = gcnew SlateClass()) << SlateClass::DeclarativeAttr<>()