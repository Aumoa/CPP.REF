// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "THGameInstance.h"

#include "THGameMode.h"
#include "PlatformMisc/PlatformConsole.h"
#include "Misc/TickScheduler.h"

using namespace std;

THGameInstance::THGameInstance() : Super()
{
	GameModeClass = TSubclassOf<ATHGameMode>::StaticClass();
	diag_tick = NewObject<TickScheduler>();

	AppName = L"GameApp";
}

THGameInstance::~THGameInstance()
{

}

//void THGameInstance::Tick()
//{
//	Super::Tick();
//}