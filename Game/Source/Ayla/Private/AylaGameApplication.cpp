// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "AylaGameApplication.h"
#include "AylaGameInstance.h"
#include "AylaGameApplication.gen.cpp"

AylaGameApplication::AylaGameApplication()
{
	GameInstanceClass = AylaGameInstance::StaticClass();
}