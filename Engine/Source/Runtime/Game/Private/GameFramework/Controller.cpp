// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "LogGame.h"

GENERATE_BODY(AController);

AController::AController() : Super()
{
	SetRootComponent(NewObject<SSceneComponent>(this));
}

void AController::Possess(APawn* InNewPawn)
{
	if (PossessedPawn != nullptr)
	{
		SE_LOG(LogController, Error, L"The controller already possessed to InNewPawn[{}]. Abort.", PossessedPawn->GetName());
		return;
	}

	PossessedPawn = InNewPawn;
	InNewPawn->PossessedBy(this);
	OnPossess(InNewPawn);
}

void AController::UnPossess()
{
	if (PossessedPawn == nullptr)
	{
		SE_LOG(LogController, Verbose, L"The controller already detached any pawn. Abort.");
		return;
	}

	OnUnPossess();
	PossessedPawn->UnPossessed();
	PossessedPawn = nullptr;
}

APawn* AController::GetPawn() const
{
	return PossessedPawn;
}