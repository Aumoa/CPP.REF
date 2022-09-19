// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameFramework/SpectatorPawn.h"
#include "Components/SphereComponent.h"
#include "SpectatorPawn.gen.cpp"

ASpectatorPawn::ASpectatorPawn()
{
	SetRootComponent(NewObject<SphereComponent>());
}