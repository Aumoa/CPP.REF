// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "CoreAObject/ObjectInitializer.h"

NObjectInitializer& NObjectInitializer::Get()
{
	static thread_local NObjectInitializer Initializer;
	return Initializer;
}

NObjectInitializer& NObjectInitializer::MarkInit(AType* InConstructType)
{
	check(ConstructType == nullptr);
	ConstructType = InConstructType;
	return *this;
}

AType* NObjectInitializer::ConsumeConstructType()
{
	AType* Consumed = nullptr;
	std::swap(Consumed, ConstructType);
	check(Consumed);
	return Consumed;
}