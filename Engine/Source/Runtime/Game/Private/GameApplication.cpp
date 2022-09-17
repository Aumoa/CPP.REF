// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameApplication.h"
#include "PlatformMisc/IPlatformWindow.h"
#include "EngineCore/GameEngine.h"
#include "Reflection/ConstructorInfo.h"
#include "Widgets/Viewports/SWindow.h"
#include "EngineCore/GameRenderSubsystem.h"
#include "Threading/GameThreads.h"
#include "GameFramework/GameInstance.h"
#include "GameApplication.gen.cpp"

constexpr LogCategory LogGameApp(TEXT("LogGameApp"));

GameApplication::GameApplication() : Super()
{
	GameInstanceClass = ::GameInstance::StaticClass();
}

int32 GameApplication::Startup(const CommandLineBuilder& args)
{
	Log::Info(LogGameApp, TEXT("Create main window."));
	Window = CreateWindow(TEXT("GameApp"));
	Window->WindowDestroyed.AddObject(this, &GameApplication::OnMainWindowDestroyed);

	Log::Info(LogGameApp, TEXT("Initialize engine."));
	Type* engineType = GetEngineType();
	checkf(engineType, TEXT("GetEngineType() return nullptr."));
	Engine = Cast<::Engine>(engineType->GetConstructors()[0]->Invoke());
	Engine->Init();

	Log::Info(LogGameApp, TEXT("Initialize slate application."));
	InitializeSlateApplication(Window);
	InitializeGameFramework();

	Log::Info(LogGameApp, TEXT("Starting main loop."));
	Window->Show(true);
	StartApplicationLoop();

	Log::Info(LogGameApp, TEXT("Game Application Shutdown."));
	return GetPlatformExitCode();
}

void GameApplication::RequestExit(int32 exitCode)
{
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

	FrameTimer.Stop();
	TimeSpan frameTime = FrameTimer.GetElapsed();
	FrameTimer.Restart();

	// Game ticks.
	Engine->ExecuteEngineLoop(frameTime);

	// Widget ticks.
	for (auto& sWindow : sWindows)
	{
		sWindow->DispatchSlateTick(frameTime);
	}

	// Render ticks.
	Engine->GetEngineSubsystem<GameRenderSubsystem>()->ExecuteRenderTicks([this]()
	{
		// Present.
		for (auto& sWindow : sWindows)
		{
			sWindow->PresentWindow();
		}
	});
}

void GameApplication::OnApplicationShutdown() noexcept
{
	FinalizeGameFramework();

	Engine->GetEngineSubsystem<GameRenderSubsystem>()->JoinRenderThread();

	sWindows.clear();

	Engine->Deinit();
	Engine = nullptr;
}

Type* GameApplication::GetEngineType()
{
	return GameEngine::StaticClass();
}

void GameApplication::InitializeSlateApplication(IPlatformWindow* initialWindow)
{
	checkf(sWindows.empty(), TEXT("SlateWindows already initialized before execute InitializeSlateApplication()."));

	std::shared_ptr<SWindow> sInitialWindow = SNew(SWindow)
		.RenderSystem(Engine->GetEngineSubsystem<GameRenderSubsystem>())
		.TargetWindow(initialWindow);

	sWindows.emplace_back(std::move(sInitialWindow));
}

void GameApplication::InitializeGameFramework()
{
	check(GameInstanceClass);
	GameInstance = Cast<::GameInstance>(GameInstanceClass->GetConstructors()[0]->Invoke());
	GameInstance->Rename(Engine, TEXT("GameInstance"));
	GameInstance->Init();
	Engine->Start(GameInstance);
}

void GameApplication::FinalizeGameFramework()
{
	GameInstance->Deinit();
}

void GameApplication::OnMainWindowDestroyed()
{
	RequestExit(0);
}