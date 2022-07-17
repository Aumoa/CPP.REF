// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameApplication.h"
#include "PlatformMisc/IPlatformWindow.h"
#include "EngineCore/GameEngine.h"
#include "Reflection/ConstructorInfo.h"
#include "GameApplication.gen.cpp"

constexpr LogCategory LogGameApp(TEXT("LogGameApp"));

GameApplication::GameApplication() : Super()
{
}

int32 GameApplication::Startup(const CommandLineBuilder& args)
{
	Log::Info(LogGameApp, TEXT("Create main window."));
	_window = CreateWindow(TEXT("GameApp"));
	_window->WindowDestroyed.AddObject(this, &GameApplication::OnMainWindowDestroyed);

	Log::Info(LogGameApp, TEXT("Initialize engine."));
	Type* engineType = GetEngineType();
	checkf(engineType, TEXT("GetEngineType() return nullptr."));
	_engine = Cast<Engine>(engineType->GetConstructors()[0]->Invoke());
	_engine->Init();

	Log::Info(LogGameApp, TEXT("Starting main loop."));
	_window->Show(true);
	StartApplicationLoop();

	Log::Info(LogGameApp, TEXT("Game Application Shutdown."));
	return GetPlatformExitCode();
}

void GameApplication::RequestExit(int32 exitCode)
{
	_engine->Deinit();
	Super::RequestExit(exitCode);
}

void GameApplication::Tick()
{
	Super::Tick();
	_engine->ExecuteEngineLoop();
}

Type* GameApplication::GetEngineType()
{
	return typeof(GameEngine);
}

void GameApplication::OnMainWindowDestroyed()
{
	RequestExit(0);
}