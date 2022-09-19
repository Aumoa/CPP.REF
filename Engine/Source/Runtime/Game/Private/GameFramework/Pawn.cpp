// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameFramework/Pawn.h"
#include "Pawn.gen.cpp"

void APawn::PossessedBy(AController* InController)
{
	check(Controller == nullptr);
	Controller = InController;
}

void APawn::UnPossessed()
{
	Controller = nullptr;
}

AController* APawn::GetController() noexcept
{
	return Controller;
}