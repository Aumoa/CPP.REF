// Copyright 2020 Aumoa.lib. All right reserved.

#include "Framework/Controller.h"

#include "Framework/Pawn.h"
#include "Logging/LogMacros.h"
#include "Logging/LogCategoryBase.h"

LogCategoryBase AController::LogController(ELogVerbosity::Verbose, L"LogController");

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

	if (inPawn == possessedPawn.Get())
	{
		SE_LOG(LogController, Verbose, L"New desired possess target is equals previous possess target. Abort.");
		return;
	}

	if (possessedPawn.IsValid)
	{
		UnPossess();
	}

	possessedPawn = inPawn;
	OnPossess(inPawn);
}

void AController::UnPossess()
{
	if (possessedPawn.IsValid)
	{
		OnUnPossess();
		possessedPawn = nullptr;
	}
}

void AController::OnPossess(APawn* inPawn)
{

}

void AController::OnUnPossess()
{

}

APawn* AController::GetPawn() const
{
	return possessedPawn.Get();
}