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

void SGameModuleSystem::LoadGameModule(String GameModuleName)
{
	String GameModulePath = GameModuleName;
	if (!Path::HasExtension(GameModulePath))
	{
		// Add .dll extension.
		GameModulePath = Path::ChangeExtension(GameModulePath, TEXT(".dll"));
	}

	std::unique_ptr Module = std::make_unique<PlatformModule>(GameModulePath);
	if (!Module->IsValid())
	{
		throw Exception(String::Format(TEXT("Could not initialize game module({})."), GameModuleName));
	}

	auto ModuleLoader = Module->GetFunctionPointer<SGameModule*(SObject*)>(TEXT("LoadGameModule"));
	if (!ModuleLoader)
	{
		throw Exception(String::Format(TEXT("The game module({}) have not LoadGameInstance function. Please add DEFINE_GAME_MODULE(YourGameInstanceClass) to your code and restart application."), GameModulePath));
	}

	GameModule = ModuleLoader(this);
	if (!GameModule)
	{
		throw Exception(String::Format(TEXT("The game module loader({}.dll@LoadGameModule()) returns nullptr."), GameModuleName));
	}

	// Keep module object while application is running.
	IApplicationInterface::Get().ConsumeModule(std::move(Module));
}

SGameInstance* SGameModuleSystem::LoadGameInstance()
{
	return GameModule->CreateGameInstance();
}