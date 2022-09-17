// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Actors/GameMode.h"
#include "GameFramework/PlayerController.h"
#include "WorldCore/World.h"
#include "GameMode.gen.cpp"

AGameMode::AGameMode()
{
	PlayerControllerClass = APlayerController::StaticClass();
}

APlayerController* AGameMode::SpawnPlayerController(World* InWorld)
{
	return InWorld->SpawnActor<APlayerController>(PlayerControllerClass);
}