// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameApplication.h"
#include "PlatformMisc/IPlatformWindow.h"
#include "EngineCore/GameEngine.h"
#include "Reflection/ConstructorInfo.h"
#include "Widgets/Viewports/SWindow.h"
#include "EngineCore/GameRenderSubsystem.h"
#include "Threading/GameThreads.h"
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

	Log::Info(LogGameApp, TEXT("Initialize slate application."));
	InitializeSlateApplication(_window);

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
	static bool bAssignGameThread = false;
	if (!bAssignGameThread)
	{
		GameThreads::InitializeGameThread();
		bAssignGameThread = true;
	}

	Super::Tick();

	// Game ticks.
	_engine->ExecuteEngineLoop();

	// Render ticks.
	_engine->GetEngineSubsystem<GameRenderSubsystem>()->ExecuteRenderTicks();

	// Flushing.
	for (auto& sWindow : _sWindows)
	{
		//sWindow->PresentWindow();
	}
}

Type* GameApplication::GetEngineType()
{
	return typeof(GameEngine);
}

void GameApplication::InitializeSlateApplication(IPlatformWindow* initialWindow)
{
	checkf(_sWindows.empty(), TEXT("SlateWindows already initialized before execute InitializeSlateApplication()."));

	std::shared_ptr<SWindow> sInitialWindow = SNew(SWindow)
		.RenderSystem(_engine->GetEngineSubsystem<GameRenderSubsystem>())
		.TargetWindow(initialWindow);

	_sWindows.emplace_back(std::move(sInitialWindow));
}

void GameApplication::OnMainWindowDestroyed()
{
	RequestExit(0);
}