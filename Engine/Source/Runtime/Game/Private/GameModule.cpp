// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameModule.h"
#include "GameInstance.h"
#include "GameEngine.h"

GENERATE_BODY(SGameModule);

SGameModule::SGameModule() : Super()
{
}

SGameModule::~SGameModule()
{
}

SGameInstance* SGameModule::CreateGameInstance()
{
	return gcnew SGameInstance();
}

SGameEngine* SGameModule::CreateGameEngine()
{
	return gcnew SGameEngine();
}

DEFINE_GAME_MODULE(SGameModule);