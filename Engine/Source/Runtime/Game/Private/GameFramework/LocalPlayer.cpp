// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameFramework/LocalPlayer.h"
#include "Level/World.h"

SLocalPlayer::SLocalPlayer() : Super()
{
}

void SLocalPlayer::Init(IFrameworkView* InFrameworkView)
{
	_FrameworkView = InFrameworkView;
}

IFrameworkView* SLocalPlayer::GetFrameworkView()
{
	return _FrameworkView;
}

void SLocalPlayer::SetPlayerController(APlayerController* InController)
{
	_PlayerController = InController;
}

APlayerController* SLocalPlayer::GetPlayerController()
{
	return _PlayerController;
}