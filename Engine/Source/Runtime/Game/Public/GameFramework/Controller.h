// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Actor.h"
#include "Controller.gen.h"

class APawn;

SCLASS()
class GAME_API AController : public AActor
{
	GENERATED_BODY()

private:
	SPROPERTY()
	APawn* PossessedPawn = nullptr;

public:
	AController();

	void Possess(APawn* InPawn);
	void UnPossess();
};