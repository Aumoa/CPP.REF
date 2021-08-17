// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameFramework/Pawn.h"
#include "LogGame.h"
#include "GameFramework/Controller.h"

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