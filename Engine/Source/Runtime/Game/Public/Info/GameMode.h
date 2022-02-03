// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Info.h"

class APlayerController;

class GAME_API AGameMode : public AInfo
{
	GENERATED_BODY(AGameMode)

public:
	SubclassOf<APlayerController> PlayerControllerClass;

public:
	AGameMode();

	APlayerController* SpawnPlayerController();
};