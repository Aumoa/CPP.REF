// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "Actors/AylaGameMode.h"
#include "Controllers/AylaPlayerController.h"
#include "AylaGameMode.gen.cpp"

AAylaGameMode::AAylaGameMode()
{
	PlayerControllerClass = AAylaPlayerController::StaticClass();
}