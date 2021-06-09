// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;
import SC.Runtime.Game;

using namespace std;

using enum ELogVerbosity;

AController::AController() : Super()
{
}

void AController::Possess(APawn* pawn)
{
	if (_possessedPawn != nullptr)
	{
		LogSystem::Log(LogController, Error, L"The controller already possessed to pawn[{}]. Abort.", _possessedPawn->GetName());
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
		LogSystem::Log(LogController, Verbose, L"The controller already detached any pawn. Abort.");
		return;
	}

	OnUnPossess();
	_possessedPawn->UnPossessed();
	_possessedPawn = nullptr;
}