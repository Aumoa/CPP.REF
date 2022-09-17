// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actor.h"
#include "GameMode.generated.h"

class APlayerController;
class World;

SCLASS()
class GAME_API AGameMode : public AActor
{
	GENERATED_BODY()

protected:
	SubclassOf<APlayerController> PlayerControllerClass;

public:
	AGameMode();

	APlayerController* SpawnPlayerController(World* InWorld);
};