// Copyright 2020 Aumoa.lib. All right reserved.

#include "Controller/THPlayerController.h"

#include "World.h"
#include "Levels/StartupLevel.h"
#include "Pawn/MyCharacter.h"

ATHPlayerController::ATHPlayerController()
{
	bShowMouseCursor = false;
}

ATHPlayerController::~ATHPlayerController()
{

}

void ATHPlayerController::BeginPlay()
{
	Super::BeginPlay();

	StartupLevel* level = Cast<StartupLevel>(GetWorld()->GetCurrentLevel());
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
}