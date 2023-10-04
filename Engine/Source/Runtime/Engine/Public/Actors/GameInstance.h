// Copyright 2020-2023 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Actor.h"
#include "GameInstance.generated.h"

ACLASS()
class ENGINE_API AGameInstance : public AActor
{
	GENERATED_BODY()

public:
	AGameInstance();

	virtual void Initialize();
	virtual void Deinitialize();
};