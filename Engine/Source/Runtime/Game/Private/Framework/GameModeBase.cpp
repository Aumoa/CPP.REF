// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Framework/GameModeBase.h"

#include "Framework/PlayerController.h"
#include "Level.h"

AGameModeBase::AGameModeBase() : Super()
{
	PlayerControllerClass = TSubclassOf<APlayerController>::StaticClass();
	StartLevelClass = TSubclassOf<GLevel>::StaticClass();
}

AGameModeBase::~AGameModeBase()
{

}