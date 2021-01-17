// Copyright 2020 Aumoa.lib. All right reserved.

#include "THGameInstance.h"

#include "THGameMode.h"
#include "Diagnostics/ScopedCycleCounterStorage.h"

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

	TH_LOG(LogTH, Verbose, String::Format("\n{0}", ScopedCycleCounterStorage::GetDiagStatics()));
}