// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameModuleSystem.h"
#include "LogGame.h"
#include "GameModule.h"

using namespace ::libty;

SGameModuleSystem::SGameModuleSystem() : Super()
{
}

SGameModuleSystem::~SGameModuleSystem()
{
}

Task<> SGameModuleSystem::StartAsync(std::stop_token CancellationToken)
{
	return Super::StartAsync(CancellationToken);
}

Task<> SGameModuleSystem::StopAsync(std::stop_token CancellationToken)
{
	if (GameModule)
	{
		GC->SuppressFinalize(GameModule);
	}

	return Super::StopAsync(CancellationToken);
}

void SGameModuleSystem::LoadGameModule(StringView GameModuleName)
{
	std::filesystem::path GameModulePath = GameModuleName;
	if (!GameModulePath.has_extension())
	{
		// Add .dll extension.
		GameModulePath.replace_extension(L".dll");
	}

	std::unique_ptr Module = std::make_unique<PlatformModule>(GameModulePath);
	if (!Module->IsValid())
	{
		throw FatalException(String::Format("Could not initialize game module({}).", String::AsMultibyte(GameModuleName)));
	}

	auto ModuleLoader = Module->GetFunctionPointer<SGameModule*(SObject*)>("LoadGameModule");
	if (!ModuleLoader)
	{
		throw FatalException(String::Format("The game module({}) have not LoadGameInstance function. Please add DEFINE_GAME_MODULE(YourGameInstanceClass) to your code and restart application.", GameModulePath.string()));
	}

	GameModule = ModuleLoader(this);
	if (!GameModule)
	{
		throw FatalException(String::Format("The game module loader({}.dll@LoadGameModule()) returns nullptr.", String::AsMultibyte(GameModuleName)));
	}

	// Keep module object while application is running.
	IApplicationInterface::Get().ConsumeModule(std::move(Module));
}

SGameInstance* SGameModuleSystem::LoadGameInstance()
{
	return GameModule->CreateGameInstance();
}