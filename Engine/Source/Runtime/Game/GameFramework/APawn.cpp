// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;
import SC.Runtime.Game;

using namespace std;

using enum ELogVerbosity;

APawn::APawn(wstring_view name) : Super(name)
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