// Copyright 2020 Aumoa.lib. All right reserved.

#include "Framework/GameModeBase.h"

#include "Framework/PlayerController.h"
#include "Level.h"

AGameModeBase::AGameModeBase() : Super()
{
	PlayerControllerClass = TSubclassOf<APlayerController>::StaticClass();
	StartLevelClass = TSubclassOf<Level>::StaticClass();
}

AGameModeBase::~AGameModeBase()
{

}