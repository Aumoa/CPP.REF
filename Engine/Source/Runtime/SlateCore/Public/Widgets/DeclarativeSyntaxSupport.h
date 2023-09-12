// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"

#define GENERATED_SLATE_BODY(WidgetType)											\
public:																				\
	using Super = This;																\
	using This = WidgetType;														\
																					\
public:																				\
	std::shared_ptr<WidgetType> SharedFromThis()									\
	{																				\
		return std::static_pointer_cast<WidgetType>(shared_from_this());			\
	}																				\
																					\
private:

namespace DeclarativeSyntaxSupports
{
	template<class TA, class TB>
	struct SelectAvoid
	{
		using Type = TA;
	};

	template<class TB>
	struct SelectAvoid<void, TB>
	{
		using Type = TB;
	};

	template<>
	struct SelectAvoid<void, void>
	{
	};
}

#define BEGIN_SLATE_ATTRIBUTE()																				\
public:																										\
	template<class TAttr = void>																			\
	struct NDeclarativeAttr : public Super::NDeclarativeAttr<TAttr>											\
	{																										\
		using This = typename DeclarativeSyntaxSupports::SelectAvoid<TAttr, NDeclarativeAttr<TAttr>>::Type;	\
																											\
		operator typename Super::template NDeclarativeAttr<>& () &&											\
		{																									\
			return reinterpret_cast<typename Super::NDeclarativeAttr<NDeclarativeAttr>&&>(*this);			\
		}																									\
																											\
		template<class UAttr>																				\
		operator NDeclarativeAttr<UAttr>& () &&																\
		{																									\
			return reinterpret_cast<NDeclarativeAttr<UAttr>&&>(*this);										\
		}

#define END_SLATE_ATTRIBUTE()																				\
	}

#define DECLARE_SLATE_ATTRIBUTE(Type, Var, ...)								\
Type _ ## Var __VA_OPT__(=) __VA_ARGS__;									\
This&& Var(const Type& InValud) &&											\
{																			\
	_ ## Var = InValud;														\
	return reinterpret_cast<This&&>(*this);									\
}																			\
template<class... TArgs> requires std::constructible_from<Type, TArgs...>	\
This&& Var(TArgs&&... args) &&												\
{																			\
	_ ## Var = { std::forward<TArgs>(args)... };							\
	return reinterpret_cast<This&&>(*this);									\
}

#define DECLARE_SLATE_CONTENT(Type)											\
Type _Content = nullptr;													\
This&& operator [](Type value) &&											\
{																			\
	_Content = value;														\
	return std::move(*reinterpret_cast<This*>(this));						\
}

#define DECLARE_SLATE_CONSTRUCTOR() void Construct(NDeclarativeAttr<>&)
#define DEFINE_SLATE_CONSTRUCTOR(SlateClass, AttrVar) void SlateClass::Construct(NDeclarativeAttr<>& AttrVar)

#define DECLARE_SLATE_SLOT_SUPPORTS(SlotClass)		\
std::vector<SlotClass> Slots;						\
This&& operator +(SlotClass&& InstanceSlot) &&		\
{													\
	Slots.emplace_back(std::move(InstanceSlot));	\
	return reinterpret_cast<This&&>(*this);			\
}

namespace DeclarativeSyntaxSupports
{
	template<class TAttribute, class TSlot>
	struct TSingleSlotSupports : public TAttribute
	{
		operator TAttribute && () { return static_cast<TAttribute&&>(std::move(*this)); }
		operator TAttribute & () { return static_cast<TAttribute&>(*this); }

	private:
		TAttribute&& operator +(TSlot&&) && { throw; }
	};
}

#define DECLARE_SLATE_SINGLE_SLOT_SUPPORTS(SlotClass)															\
SlotClass SingleSlot;																							\
DeclarativeSyntaxSupports::TSingleSlotSupports<This, SlotClass>&& operator +(SlotClass&& InstanceSlot) &&		\
{																												\
	SingleSlot = std::move(InstanceSlot);																		\
	return std::move(static_cast<DeclarativeSyntaxSupports::TSingleSlotSupports<This, SlotClass>&>(*this));		\
}

#define SNew(SlateClass) (std::make_shared<SlateClass>()) << SlateClass::NDeclarativeAttr<SlateClass::NDeclarativeAttr<>>()
#define SAssignNew(Var, SlateClass) (Var = std::make_shared<SlateClass>()) << SlateClass::NDeclarativeAttr<SlateClass::NDeclarativeAttr<>>()