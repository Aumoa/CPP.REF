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

#define BEGIN_SLATE_ATTRIBUTE(WidgetType)											\
public:																				\
	struct NDeclarativeAttr : public Super::NDeclarativeAttr						\
	{																				\
		using This = NDeclarativeAttr;												\
																					\
		operator typename Super::NDeclarativeAttr&& () &							\
		{																			\
			return std::move(static_cast<typename Super::NDeclarativeAttr&>(*this));\
		}

#define END_SLATE_ATTRIBUTE()														\
	}

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
Type _Content = nullptr;													\
This&& operator [](Type value) &&											\
{																			\
	_Content = value;														\
	return std::move(*static_cast<This*>(this));							\
}

#define DECLARE_SLATE_CONSTRUCTOR() void Construct(NDeclarativeAttr&)
#define DEFINE_SLATE_CONSTRUCTOR(SlateClass, AttrVar) void SlateClass::Construct(NDeclarativeAttr& AttrVar)

#define DECLARE_SLATE_SLOT_SUPPORTS(SlotClass)		\
std::vector<SlotClass> Slots;						\
This&& operator +(SlotClass&& InstanceSlot) &&		\
{													\
	Slots.emplace_back(std::move(InstanceSlot));	\
	return std::move(*static_cast<This*>(this));	\
}

#define SNew(SlateClass) (std::make_shared<SlateClass>()) << SlateClass::NDeclarativeAttr()
#define SAssignNew(Var, SlateClass) (Var = std::make_shared<SlateClass>()) << SlateClass::NDeclarativeAttr()