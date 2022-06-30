// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Info/GameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/LocalPlayer.h"
#include "Level/Level.h"
#include "Level/World.h"

using namespace ::libty;

AGameMode::AGameMode() : Super()
	, PlayerControllerClass(APlayerController::TypeId)
{
}

APlayerController* AGameMode::SpawnPlayerController()
{
	SLevel* const Level = GetLevel();
	APlayerController* PlayerController = Cast<APlayerController>(Level->SpawnActor(PlayerControllerClass, true));

	SWorld* World = GetWorld();
	SLocalPlayer* LocalPlayer = World->GetLocalPlayer();

	//LocalPlayer->SetPlayerController(PlayerController);
	return PlayerController;
}