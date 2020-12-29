// Copyright 2020 Aumoa.lib. All right reserved.

#include "THGameInstance.h"

#include "Levels/StartupLevel.h"

THGameInstance::THGameInstance()
{

}

THGameInstance::~THGameInstance()
{

}

TRefPtr<String> THGameInstance::ToString() const
{
	return L"Touhou Hourai";
}

Level* THGameInstance::GetStartupLevel()
{
	if (!startupLevel.IsValid)
	{
		startupLevel = NewObject<StartupLevel>();
	}
	return startupLevel.Get();
}