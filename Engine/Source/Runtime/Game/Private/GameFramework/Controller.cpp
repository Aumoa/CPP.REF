// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameFramework/Controller.h"
#include "LogGame.h"
#include "GameFramework/Pawn.h"

AController::AController() : Super()
{
	SetRootComponent(NewObject<SceneComponent>());
}

void AController::Possess(APawn* pawn)
{
	if (_possessedPawn != nullptr)
	{
		SE_LOG(LogController, Error, L"The controller already possessed to pawn[{}]. Abort.", _possessedPawn->GetName());
		return;
	}

	_possessedPawn = pawn;
	pawn->PossessedBy(this);
	OnPossess(pawn);
}

void AController::UnPossess()
{
	if (_possessedPawn == nullptr)
	{
		SE_LOG(LogController, Verbose, L"The controller already detached any pawn. Abort.");
		return;
	}

	OnUnPossess();
	_possessedPawn->UnPossessed();
	_possessedPawn = nullptr;
}

APawn* AController::GetPawn() const
{
	return _possessedPawn;
}