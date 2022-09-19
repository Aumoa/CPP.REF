// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Controller.gen.cpp"

AController::AController()
{
}

void AController::Possess(APawn* InPawn)
{
	UnPossess();

	if (InPawn)
	{
		PossessedPawn = InPawn;
		InPawn->PossessedBy(this);
	}
}

void AController::UnPossess()
{
	if (PossessedPawn)
	{
		PossessedPawn->UnPossessed();
		PossessedPawn = nullptr;
	}
}