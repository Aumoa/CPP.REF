// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EngineSubsystems/GameModuleSystem.h"
#include "PlatformMisc/PlatformModule.h"
#include "LogGame.h"

GameModuleSystem::GameModuleSystem() : Super()
{
}

GameModuleSystem::~GameModuleSystem()
{
}

void GameModuleSystem::Init()
{
	Super::Init();
}

void GameModuleSystem::LoadGameModule(std::wstring_view gameModuleName)
{
	std::filesystem::path gameModulePath = gameModuleName;
	if (!gameModulePath.has_extension())
	{
		// Add .dll extension.
		gameModulePath.replace_extension(L".dll");
	}

	_module = CreateSubobject<PlatformModule>(gameModulePath);
	if (!_module->IsValid())
	{
		SE_LOG(LogModule, Fatal, L"Could not initialize game module({}).", gameModuleName);
		return;
	}

	_gameInstanceLoader = _module->GetFunctionPointer<GameInstance*()>("LoadGameInstance");
	if (!_gameInstanceLoader)
	{
		SE_LOG(LogModule, Fatal, L"The game module({}) have not LoadGameInstance function. Please add DEFINE_GAME_MODULE(YourGameInstanceClass) to your code and restart application.");
		return;
	}
}

GameInstance* GameModuleSystem::LoadGameInstance()
{
	return _gameInstanceLoader();
}