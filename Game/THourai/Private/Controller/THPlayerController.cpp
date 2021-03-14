// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Controller/THPlayerController.h"

#include "World.h"
#include "Levels/StartupLevel.h"
#include "Misc/TickScheduler.h"
#include "PlatformMisc/PlatformConsole.h"

using namespace std;

ATHPlayerController::ATHPlayerController()
{
	bShowMouseCursor = false;

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

ATHPlayerController::~ATHPlayerController()
{

}

void ATHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	GStartupLevel* level = Cast<GStartupLevel>(GetWorld()->GetCurrentLevel());
	if (level == nullptr)
	{
		TH_LOG(LogTH, Error, L"Current level must StartupLevel class. (TEST IMPLEMENT)");
		return;
	}

	APawn* actor = level->GetPersistentActor();
	Possess(actor);
}

void ATHPlayerController::Tick(Seconds deltaTime)
{
	Super::Tick(deltaTime);

	diag_tick->Tick(deltaTime);
}