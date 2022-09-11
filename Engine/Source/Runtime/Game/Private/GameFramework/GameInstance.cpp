// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameFramework/GameInstance.h"
#include "GameInstance.gen.cpp"

constexpr LogCategory LogGameInstance(TEXT("LogGameInstance"));

GameInstance::GameInstance()
{
}

void GameInstance::Init()
{
	Log::Info(LogGameInstance, TEXT("GameInstance::Init() called."));
}

void GameInstance::Deinit()
{
	Log::Info(LogGameInstance, TEXT("GameInstance::Deinit() called."));
}