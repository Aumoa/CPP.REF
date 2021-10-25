// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameEngine.h"
#include "LogGame.h"
#include "GameInstance.h"
#include "IFrameworkView.h"
#include "CoreDelegates.h"
#include "PlatformMisc/PlatformModule.h"
#include "Threading/Thread.h"
#include "Level/World.h"
#include "Application/SlateApplication.h"
#include "GameThreads/RenderThread.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "EngineSubsystems/GameModuleSystem.h"
#include "EngineSubsystems/GameAssetSystem.h"
#include "EngineSubsystems/GameLevelSystem.h"
#include "EngineSubsystems/GamePlayerSystem.h"
#include "EngineSubsystems/GameInputSystem.h"

SGameEngine* GEngine = nullptr;

SGameEngine::SGameEngine() : Super()
{
}

SGameEngine::~SGameEngine()
{
}

bool SGameEngine::InitEngine()
{
	GEngine = this;
	InitializeSubsystems();
	CoreDelegates::PostEngineInit.Invoke();

	SThread::GetCurrentThread()->SetFriendlyName(L"[Main Thread]");
	return true;
}

void SGameEngine::SetupFrameworkView(IFrameworkView* frameworkView)
{
	GetEngineSubsystem<SGameRenderSystem>()->SetupFrameworkView(frameworkView);
	auto PlayerSystem = GetEngineSubsystem<SGamePlayerSystem>();
	PlayerSystem->SpawnLocalPlayer(frameworkView);

	SE_LOG(LogEngine, Info, L"Register engine tick.");
	frameworkView->Idle.AddSObject(this, &SGameEngine::TickEngine);

	SlateApplication = NewObject<SSlateApplication>();
	SlateApplication->InitWindow(PlayerSystem->GetLocalPlayer(), frameworkView);
}

bool SGameEngine::LoadGameModule(std::wstring_view moduleName)
{
	SGameModuleSystem* system = GetEngineSubsystem<SGameModuleSystem>();
	system->LoadGameModule(moduleName);
	if (GameInstance = system->LoadGameInstance(); GameInstance == nullptr)
	{
		SE_LOG(LogEngine, Fatal, L"LoadGameInstance function from {} module return nullptr.", moduleName);
		return false;
	}

	SWorld* GameWorld = GetEngineSubsystem<SGameLevelSystem>()->GetGameWorld();
	GameInstance->SetOuter(GameWorld);
	if (!GameInstance->StartupLevel.IsValid())
	{
		SE_LOG(LogEngine, Fatal, L"SGameInstance::StartupLevel is not specified.");
		return false;
	}

	if (!GetEngineSubsystem<SGameLevelSystem>()->OpenLevel(GameInstance->StartupLevel))
	{
		SE_LOG(LogEngine, Fatal, L"Could not startup level.");
		return false;
	}

	return true;
}

void SGameEngine::Shutdown()
{
	if (SlateApplication)
	{
		DestroyObject(SlateApplication);
		SlateApplication = nullptr;
	}

	for (auto& System : _Subsystems)
	{
		System->Deinit();
	}
}

int32 SGameEngine::InvokedMain(IFrameworkView* frameworkView, std::wstring_view gameModule)
{
	CoreDelegates::BeginMainInvoked.Invoke();

	// Create GameEngine instance and initialize it.
	if (!InitEngine())
	{
		SE_LOG(LogEngine, Fatal, L"Could not initialize engine.");
		return -1;
	}

	// Setup framework view.
	SetupFrameworkView(frameworkView);

	// Load game module.
	if (!LoadGameModule(gameModule))
	{
		return -1;
	}

	// Start application now!
	frameworkView->SetFrameworkTitle(GameInstance->GetApplicationName());
	GameInstance->Init();
	frameworkView->Start();

	Shutdown();
	return 0;
}

SGameInstance* SGameEngine::GetGameInstance()
{
	return GameInstance;
}

void SGameEngine::InitializeSubsystems()
{
	SE_LOG(LogEngine, Verbose, L"Initialize subsystems.");
	_Subsystems.emplace_back(NewObject<SGameRenderSystem>())->Init();
	_Subsystems.emplace_back(NewObject<SGameAssetSystem>())->Init();
	_Subsystems.emplace_back(NewObject<SGameModuleSystem>())->Init();
	_Subsystems.emplace_back(NewObject<SGameLevelSystem>())->Init();
	_Subsystems.emplace_back(NewObject<SGamePlayerSystem>())->Init();
	_Subsystems.emplace_back(NewObject<SGameInputSystem>())->Init();
}

void SGameEngine::TickEngine()
{
	using namespace std::chrono;

	auto Tick = _TickCalc.DoCalc();
	SystemsTick(Tick);
	GameTick(Tick);
	RenderTick(Tick);
}

void SGameEngine::SystemsTick(std::chrono::duration<float> InDeltaTime)
{
	for (auto& System : _Subsystems)
	{
		System->Tick(InDeltaTime.count());
	}
}

void SGameEngine::GameTick(std::chrono::duration<float> InDeltaTime)
{
	SWorld* GameWorld = GetEngineSubsystem<SGameLevelSystem>()->GetGameWorld();
	GameWorld->LevelTick(InDeltaTime.count());
}

void SGameEngine::RenderTick(std::chrono::duration<float> InDeltaTime)
{
	GetEngineSubsystem<SGameRenderSystem>()->ExecuteRenderThread();
}