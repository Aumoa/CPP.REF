// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "LogGame.h"

using namespace ::libty;

APawn::APawn() : Super()
{
}

void APawn::PossessedBy(AController* controller)
{
	if (_controller != nullptr)
	{
		SE_LOG(LogPawn, Error, L"The pawn already possessed by controller[{}]. Abort.", controller->GetName());
		return;
	}

	_controller = controller;
}

void APawn::UnPossessed()
{
	_controller = nullptr;
}