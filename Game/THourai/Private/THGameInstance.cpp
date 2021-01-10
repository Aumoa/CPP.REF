// Copyright 2020 Aumoa.lib. All right reserved.

#include "THGameInstance.h"

#include "THGameMode.h"

THGameInstance::THGameInstance()
{
	GameModeClass = TSubclassOf<ATHGameMode>::StaticClass();
}

THGameInstance::~THGameInstance()
{

}

TRefPtr<String> THGameInstance::ToString() const
{
	return L"Touhou Hourai";
}