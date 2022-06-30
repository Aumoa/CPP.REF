// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameFramework/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Level/World.h"

using namespace ::libty;

SLocalPlayer::SLocalPlayer()
	: Super()
{
}

void SLocalPlayer::SetPlayerController(APlayerController* InController)
{
	PlayerController = InController;
}

APlayerController* SLocalPlayer::GetPlayerController()
{
	return PlayerController;
}