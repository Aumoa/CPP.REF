// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "LogGame.h"

using namespace ::libty;

AController::AController() : Super()
{
	SetRootComponent(gcnew SSceneComponent());
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