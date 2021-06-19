// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameMode.h"
#include "GameFramework/PlayerController.h"

using namespace std;

AGameMode::AGameMode() : Super()
	, PlayerControllerClass(SubclassOf<APlayerController>::StaticClass())
{
}