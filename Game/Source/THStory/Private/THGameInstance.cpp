// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "THGameInstance.h"
#include "Levels/StartupLevel.h"

THGameInstance::THGameInstance() : Super()
{
	StartupLevel = SubclassOf<::StartupLevel>::StaticClass();
}