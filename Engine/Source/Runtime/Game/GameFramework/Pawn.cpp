// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "Pawn.h"
#include "LogGame.h"
#include "GameFramework/Controller.h"

using namespace std;

using enum ELogVerbosity;

APawn::APawn() : Super()
{
}

void APawn::PossessedBy(AController* controller)
{
	if (_controller != nullptr)
	{
		LogSystem::Log(LogPawn, Error, L"The pawn already possessed by controller[{}]. Abort.", controller->GetName());
		return;
	}

	_controller = controller;
}

void APawn::UnPossessed()
{
	_controller = nullptr;
}