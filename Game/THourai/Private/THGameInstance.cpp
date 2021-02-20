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

	diag_tick->Tick(deltaTime);
}