// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Info/GameMode.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/LocalPlayer.h"
#include "Level/World.h"
#include "EngineSubsystems/GamePlayerSystem.h"

AGameMode::AGameMode() : Super()
	, PlayerControllerClass(APlayerController::StaticClass())
{
}

APlayerController* AGameMode::SpawnPlayerController()
{
	SWorld* const World = GetWorld();
	APlayerController* PlayerController = World->SpawnActor(PlayerControllerClass);

	auto* System = GEngine->GetEngineSubsystem<SGamePlayerSystem>();
	SLocalPlayer* LocalPlayer = System->GetLocalPlayer();

	PlayerController->SetOuter(LocalPlayer);
	return PlayerController;
}