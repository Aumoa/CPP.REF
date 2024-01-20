// Copyright 2020-2023 Aumoa.lib. All right reserved.

#include "GameEngine.h"
#include "Global.h"
#include "AssemblyUtility/GameAssembly.h"

GameEngine::GameEngine()
{
}

GameEngine::~GameEngine() noexcept
{
}

void GameEngine::Initialize()
{
	StartupGameAssembly();
}

void GameEngine::Deinitialize()
{
}

void GameEngine::StartupGameAssembly()
{
	String AssemblyName;
	if (CommandLine::TryGetValue(TEXT("GameAssembly"), AssemblyName) == false)
	{
		throw InvalidOperationException(TEXT("GameAssembly is not specified."));
	}

	auto AssemblyDir = Global::GetGameDirectory().GetChild(TEXT("Binaries")).GetChild(PLATFORM_STRING).GetChild(CONFIG_STRING);
	auto Library = std::make_shared<DynamicLibrary>(AssemblyDir.GetFile(AssemblyName));

	Func<GameAssembly*> AssemblyLoader = Library->LoadFunction<GameAssembly*>(TEXT("LoadAssemblyForCurrentModule"));
	if (AssemblyLoader.IsBound() == false)
	{
		throw InvalidOperationException(String::Format(TEXT("Assembly {0} is not provide LoadAssemblyFor{0} function. Please use DEFINE_GAME_ASSEMBLY_LOADER macro to implement loader function for export."), AssemblyName));
	}

	auto LoadedAssembly = std::shared_ptr<GameAssembly>(AssemblyLoader.Invoke());
	String ModuleName = LoadedAssembly->GetModuleName();

	AssemblyLib = std::move(Library);
	Assembly = std::move(LoadedAssembly);
}