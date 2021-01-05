// Copyright 2020 Aumoa.lib. All right reserved.

#include "Framework/Controller.h"

#include "Framework/Pawn.h"

AController::AController()
{

}

AController::~AController()
{

}

void AController::Possess(APawn* inPawn)
{
	if (inPawn == nullptr)
	{
		UnPossess();
		return;
	}

	if (possessedPawn.IsValid)
	{
		UnPossess();
	}

	possessedPawn = inPawn;
}

void AController::UnPossess()
{
	if (possessedPawn.IsValid)
	{
		possessedPawn = nullptr;
	}
}

APawn* AController::GetPawn() const
{
	return possessedPawn.Get();
}