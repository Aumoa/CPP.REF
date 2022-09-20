// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Actor.h"
#include "Pawn.gen.h"

class AController;

SCLASS()
class GAME_API APawn : public AActor
{
	GENERATED_BODY()

private:
	SPROPERTY()
	AController* Controller = nullptr;

public:
	virtual void PossessedBy(AController* InController);
	virtual void UnPossessed();

	AController* GetController() noexcept;
	template<std::derived_from<AController> TController>
	TController* GetController() noexcept { return Cast<TController>(GetController()); }
};