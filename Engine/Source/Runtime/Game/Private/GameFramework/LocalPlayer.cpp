// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameFramework/LocalPlayer.h"
#include "Level/World.h"
#include "Application/SlateApplication.h"
#include "Application/Window.h"

SLocalPlayer::SLocalPlayer() : Super()
{
}

void SLocalPlayer::SetPlayerController(APlayerController* InController)
{
	_PlayerController = InController;
}

APlayerController* SLocalPlayer::GetPlayerController()
{
	return _PlayerController;
}

SSlateApplication* SLocalPlayer::GetSlateApplication()
{
	return Cast<SSlateApplication>(GetOuter());
}

SViewport* SLocalPlayer::GetGameViewport()
{
	SSlateApplication* App = GetSlateApplication();
	SWindow* Window = App->GetCoreWindow();
	return Window->GetGameViewport();
}