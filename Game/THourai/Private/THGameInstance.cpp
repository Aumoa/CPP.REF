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

	TickScheduleTaskInfo taskInfo;
	taskInfo.Delay = 5000ms;
	taskInfo.InitDelay = 1000ms;
	taskInfo.Task = []()
	{
		PlatformConsole::WriteLine(L"======= STATS CYCLE_COUNTER BEGIN =======");
		PlatformConsole::WriteLine(L"");

		auto container = CycleStatsGroup::GetAllStatsGroup();
		for (auto& item : container)
		{
			TRefPtr<String> item_str = item->ToString();
			if (item_str.IsValid)
			{
				PlatformConsole::WriteLine(item_str);
			}
		}

		PlatformConsole::WriteLine(L"");
		PlatformConsole::WriteLine(L"======= STATS CYCLE_COUNTER END =======");
	};

	diag_tick->AddSchedule(taskInfo);

	AppName = L"GameApp";
}

THGameInstance::~THGameInstance()
{

}

void THGameInstance::Tick()
{
	Super::Tick();

	diag_tick->Tick(Seconds(0ms));
}