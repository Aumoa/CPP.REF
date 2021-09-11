// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EngineSubsystems/GameModuleSystem.h"
#include "PlatformMisc/PlatformModule.h"
#include "LogGame.h"
#include "GameModule.h"

GameModuleSystem::GameModuleSystem() : Super()
{
}

GameModuleSystem::~GameModuleSystem()
{
}

void GameModuleSystem::LoadGameModule(std::wstring_view gameModuleName)
{
	std::filesystem::path gameModulePath = gameModuleName;
	if (!gameModulePath.has_extension())
	{
		// Add .dll extension.
		gameModulePath.replace_extension(L".dll");
	}

	_module = NewObject<PlatformModule>(gameModulePath);
	if (!_module->IsValid())
	{
		SE_LOG(LogModule, Fatal, L"Could not initialize game module({}).", gameModuleName);
		return;
	}

	auto _gameModuleLoader = _module->GetFunctionPointer<GameModule*()>("LoadGameModule");
	if (!_gameModuleLoader)
	{
		SE_LOG(LogModule, Fatal, L"The game module({}) have not LoadGameInstance function. Please add DEFINE_GAME_MODULE(YourGameInstanceClass) to your code and restart application.");
		return;
	}

	_gameModule = _gameModuleLoader();
	if (!_gameModule)
	{
		SE_LOG(LogModule, Fatal, L"The game module loader({}.dll@LoadGameModule()) returns nullptr.", gameModuleName);
		return;
	}
}

GameInstance* GameModuleSystem::LoadGameInstance()
{
	return _gameModule->CreatePrimaryGameModule();
}