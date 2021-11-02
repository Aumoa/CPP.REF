// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Info/GameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/LocalPlayer.h"
#include "Level/Level.h"
#include "Level/World.h"

AGameMode::AGameMode() : Super()
	, PlayerControllerClass(APlayerController::StaticClass())
{
}

APlayerController* AGameMode::SpawnPlayerController()
{
	SLevel* const Level = GetLevel();
	APlayerController* PlayerController = Cast<APlayerController>(Level->SpawnActor(PlayerControllerClass, true));

	SWorld* World = GetWorld();
	SLocalPlayer* LocalPlayer = World->GetLocalPlayer();

	LocalPlayer->SetPlayerController(PlayerController);
	return PlayerController;
}