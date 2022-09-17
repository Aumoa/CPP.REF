// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "AylaGameInstance.h"
#include "Levels/GameStarterLevel.h"
#include "AylaGameInstance.gen.cpp"

AylaGameInstance::AylaGameInstance()
{
	StartupLevel = GameStarterLevel::StaticClass();
}