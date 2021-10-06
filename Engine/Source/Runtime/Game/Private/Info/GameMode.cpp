// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Info/GameMode.h"
#include "GameFramework/PlayerController.h"

AGameMode::AGameMode() : Super()
	, PlayerControllerClass(APlayerController::StaticClass())
{
}