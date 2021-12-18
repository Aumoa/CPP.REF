// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameModuleSystem.h"
#include "PlatformMisc/PlatformModule.h"
#include "LogGame.h"
#include "GameModule.h"

GENERATE_BODY(SGameModuleSystem);

SGameModuleSystem::SGameModuleSystem() : Super()
{
}

SGameModuleSystem::~SGameModuleSystem()
{
}

void SGameModuleSystem::Init()
{
}

void SGameModuleSystem::Deinit()
{
}

void SGameModuleSystem::LoadGameModule(std::wstring_view GameModuleName)
{
	std::filesystem::path GameModulePath = GameModuleName;
	if (!GameModulePath.has_extension())
	{
		// Add .dll extension.
		GameModulePath.replace_extension(L".dll");
	}

	Module = std::make_unique<PlatformModule>(GameModulePath);
	if (!Module)
	{
		SE_LOG(LogModule, Fatal, L"Could not initialize game module({}).", GameModuleName);
		return;
	}

	auto ModuleLoader = Module->GetFunctionPointer<SGameModule*(SObject*)>("LoadGameModule");
	if (!ModuleLoader)
	{
		SE_LOG(LogModule, Fatal, L"The game module({}) have not LoadGameInstance function. Please add DEFINE_GAME_MODULE(YourGameInstanceClass) to your code and restart application.");
		return;
	}

	GameModule = ModuleLoader(this);
	if (!GameModule)
	{
		SE_LOG(LogModule, Fatal, L"The game module loader({}.dll@LoadGameModule()) returns nullptr.", GameModuleName);
		return;
	}
}

SGameInstance* SGameModuleSystem::LoadGameInstance()
{
	return GameModule->CreateGameInstance();
}