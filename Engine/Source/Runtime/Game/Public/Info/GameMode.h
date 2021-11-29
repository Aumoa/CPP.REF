// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Info.h"
#include "SubclassOf.h"

class APlayerController;

/// <summary>
/// Represents game mode placed to world.
/// </summary>
class GAME_API AGameMode : public AInfo
{
	GENERATED_BODY(AGameMode)

public:
	SubclassOf<APlayerController> PlayerControllerClass;

public:
	AGameMode();

	APlayerController* SpawnPlayerController();
};