// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Controller/THPlayerController.h"

ATHPlayerController::ATHPlayerController() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATHPlayerController::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
}