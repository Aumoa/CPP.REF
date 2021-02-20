// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Framework/Pawn.h"

#include "Logging/LogMacros.h"
#include "Framework/Controller.h"

APawn::APawn() : Super()
	, myController(nullptr)
{

}

APawn::~APawn()
{

}

void APawn::SetupPlayerInputComponent(InputComponent* inPlayerInput)
{

}

void APawn::PossessedBy(AController* inNewController)
{
	if (myController != nullptr)
	{
		SE_LOG(LogPawn, Error, L"Pawn is already possessed by {0}. Abort.", myController->ToString());
		return;
	}

	myController = inNewController;
}

void APawn::UnPossessed()
{
	myController = nullptr;
}

AController* APawn::GetController() const
{
	return myController;
}