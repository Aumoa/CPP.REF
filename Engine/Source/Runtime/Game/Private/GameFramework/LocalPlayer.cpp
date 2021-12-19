// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameFramework/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Level/World.h"
#include "Application/SlateApplication.h"
#include "Application/Window.h"

GENERATE_BODY(SLocalPlayer);

SLocalPlayer::SLocalPlayer(SSlateApplication* SlateApp) : Super()
	, SlateApp(SlateApp)
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

SSlateApplication* SLocalPlayer::GetSlateApplication()
{
	return SlateApp;
}

SViewport* SLocalPlayer::GetGameViewport()
{
	SSlateApplication* App = GetSlateApplication();
	SWindow* Window = App->GetCoreWindow();
	return Window->GetGameViewport();
}