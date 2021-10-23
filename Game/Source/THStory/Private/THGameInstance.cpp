// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "THGameInstance.h"
#include "Level/StartupLevel.h"

STHGameInstance::STHGameInstance() : Super()
{
	StartupLevel = ::SStartupLevel::StaticClass();
}

void STHGameInstance::Init()
{
	Super::Init();
}