// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameApplication.h"
#include "PlatformMisc/IPlatformWindow.h"
#include "EngineCore/GameEngine.h"
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
	_engine = gcnew GameEngine();
	_engine->Init();

	Log::Info(LogGameApp, TEXT("Starting main loop."));
	_window->Show(true);
	StartApplicationLoop();

	Log::Info(LogGameApp, TEXT("Game Application Shutdown."));
	return GetPlatformExitCode();
}

void GameApplication::Tick()
{
	Super::Tick();
	_engine->ExecuteEngineLoop();
}

void GameApplication::OnMainWindowDestroyed()
{
	RequestExit(0);
}