// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameModule.h"
#include "GameInstance.h"
#include "GameEngine.h"

SGameModule::SGameModule() : Super()
{
}

SGameModule::~SGameModule()
{
}

SGameInstance* SGameModule::CreatePrimaryGameModule()
{
	return NewObject<SGameInstance>();
}

SGameEngine* SGameModule::CreateGameEngine()
{
	return NewObject<SGameEngine>();
}

DEFINE_GAME_MODULE(SGameModule);