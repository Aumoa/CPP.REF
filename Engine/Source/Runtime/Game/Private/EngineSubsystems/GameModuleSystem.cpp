// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EngineSubsystems/GameModuleSystem.h"
#include "PlatformMisc/PlatformModule.h"
#include "LogGame.h"
#include "GameModule.h"

SGameModuleSystem::SGameModuleSystem() : Super()
{
}

SGameModuleSystem::~SGameModuleSystem()
{
}

void SGameModuleSystem::LoadGameModule(std::wstring_view gameModuleName)
{
	std::filesystem::path gameModulePath = gameModuleName;
	if (!gameModulePath.has_extension())
	{
		// Add .dll extension.
		gameModulePath.replace_extension(L".dll");
	}

	_module = NewObject<SPlatformModule>(gameModulePath);
	if (!_module->IsValid())
	{
		SE_LOG(LogModule, Fatal, L"Could not initialize game module({}).", gameModuleName);
		return;
	}

	auto loader = _module->GetFunctionPointer<SGameModule*()>("LoadGameModule");
	if (!loader)
	{
		SE_LOG(LogModule, Fatal, L"The game module({}) have not LoadGameInstance function. Please add DEFINE_GAME_MODULE(YourGameInstanceClass) to your code and restart application.");
		return;
	}

	_gameModule = loader();
	if (!_gameModule)
	{
		SE_LOG(LogModule, Fatal, L"The game module loader({}.dll@LoadGameModule()) returns nullptr.", gameModuleName);
		return;
	}
}

SGameInstance* SGameModuleSystem::LoadGameInstance()
{
	return _gameModule->CreatePrimaryGameModule();
}