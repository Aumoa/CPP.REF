// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "THGameMode.h"
#include "Controller/THPlayerController.h"

ATHGameMode::ATHGameMode() : Super()
{
	PlayerControllerClass = ATHPlayerController::StaticClass();
}