// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player.h"

class APlayerController;
class SViewport;
class SSlateApplication;

class GAME_API SLocalPlayer : public SPlayer
{
	GENERATED_BODY(SLocalPlayer)

private:
	SPROPERTY(SlateApp)
	SSlateApplication* SlateApp = nullptr;
	SPROPERTY(PlayerController)
	APlayerController* PlayerController = nullptr;

public:
	SLocalPlayer(SSlateApplication* SlateApp);

	void SetPlayerController(APlayerController* InController);
	APlayerController* GetPlayerController();

	SSlateApplication* GetSlateApplication();
	SViewport* GetGameViewport();
};