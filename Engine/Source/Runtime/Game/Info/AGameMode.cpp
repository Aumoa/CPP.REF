// Copyright 2020-2021 Aumoa.lib. All right reserved.

import std.core;
import SC.Runtime.Game;

using namespace std;

AGameMode::AGameMode() : Super()
	, PlayerControllerClass(SubclassOf<APlayerController>::StaticClass())
{
}