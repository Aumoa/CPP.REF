// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Player.h"
#include "SubclassOf.h"

interface IFrameworkView;
class APlayerController;

class GAME_API SLocalPlayer : public SPlayer
{
	GENERATED_BODY(SLocalPlayer)

private:
	APlayerController* _PlayerController = nullptr;
	IFrameworkView* _FrameworkView = nullptr;

public:
	SLocalPlayer();

	void Init(IFrameworkView* InFrameworkView);
	IFrameworkView* GetFrameworkView();

	void SetPlayerController(APlayerController* InController);
	APlayerController* GetPlayerController();
};