// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "AppEngineModule.h"
#include "AppEngine.h"

SAppEngineModule::SAppEngineModule() : Super()
{
}

SAppEngineModule::~SAppEngineModule()
{
}

SGameEngine* SAppEngineModule::CreateGameEngine()
{
	return NewObject<SAppEngine>();
}

DEFINE_GAME_MODULE(SAppEngineModule);