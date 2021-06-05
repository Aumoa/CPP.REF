// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Game;

using namespace std;
using namespace std::chrono;

AActor::AActor(wstring_view name) : Super(name)
	, PrimaryActorTick(this)
{
}

void AActor::TickActor(duration<float> elapsedTime, ActorTickFunction* tickFunction)
{
}