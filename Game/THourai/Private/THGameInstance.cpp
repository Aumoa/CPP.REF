// Copyright 2020 Aumoa.lib. All right reserved.

#include "THGameInstance.h"

#include "THGameMode.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "PlatformMisc/PlatformConsole.h"
#include "Framework/PlayerController.h"

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

void THGameInstance::Tick(Seconds deltaTime)
{
	Super::Tick(deltaTime);

	PlatformConsole::WriteLine(STATGROUP_APlayerController.ToString());
}