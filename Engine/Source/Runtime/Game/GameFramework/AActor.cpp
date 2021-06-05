// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Core;
import SC.Runtime.Game;

using namespace std;
using namespace std::chrono;

using enum ELogVerbosity;

void AActor::ActorTickFunction::ExecuteTick(duration<float> elapsedTime)
{
	if (_target == nullptr)
	{
		LogSystem::Log(LogTicking, Error, L"Target is nullptr.");
		return;
	}

	if (_target->HasBegunPlay() && _target->IsActive())
	{
		_target->TickActor(elapsedTime, this);
	}
}

AActor::AActor(wstring_view name) : Super(name)
	, PrimaryActorTick(this)
{
}

void AActor::TickActor(duration<float> elapsedTime, ActorTickFunction* tickFunction)
{
}

void AActor::SetActive(bool bActive)
{
	if (_bActive != bActive)
	{
		_bActive = bActive;
		if (_bActive)
		{
			Activated.Invoke();
		}
		else
		{
			Inactivated.Invoke();
		}
	}
}