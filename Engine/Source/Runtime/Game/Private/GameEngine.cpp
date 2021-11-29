// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameEngine.h"
#include "LogGame.h"
#include "GameInstance.h"
#include "CoreDelegates.h"
#include "RenderThread.h"
#include "Threading/Thread.h"
#include "Level/World.h"
#include "Application/SlateApplication.h"
#include "EngineSubsystems/GameEngineSubsystem.h"
#include "EngineSubsystems/GameModuleSystem.h"
#include "EngineSubsystems/GameLevelSystem.h"
#include "EngineSubsystems/GameRenderSystem.h"

SGameEngine* GEngine = nullptr;

SGameEngine::SGameEngine() : Super()
{
}

SGameEngine::~SGameEngine()
{
}

bool SGameEngine::InitEngine(IApplicationInterface* InApplication)
{
	GEngine = this;

	SlateApplication = NewObject<SSlateApplication>();
	SlateApplication->Init(InApplication);

	InitializeSubsystems();
	CoreDelegates::PostEngineInit.Invoke();

	SThread::GetCurrentThread()->SetFriendlyName(L"[Main Thread]");
	return true;
}

bool SGameEngine::LoadGameModule(std::wstring_view InModuleName)
{
	SGameModuleSystem* ModuleSystem = GetEngineSubsystem<SGameModuleSystem>();
	ModuleSystem->LoadGameModule(InModuleName);
	if (GameInstance = ModuleSystem->LoadGameInstance(); GameInstance == nullptr)
	{
		SE_LOG(LogEngine, Fatal, L"LoadGameInstance function from {} module return nullptr.", InModuleName);
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
	RenderThread::Shutdown();

	if (SlateApplication)
	{
		DestroyObject(SlateApplication);
		SlateApplication = nullptr;
	}

	for (auto& System : Subsystems)
	{
		System->Deinit();
	}
}

int32 SGameEngine::GuardedMain(IApplicationInterface* InApplication, std::wstring_view gameModule)
{
	CoreDelegates::BeginMainInvoked.Invoke();

	// Create GameEngine instance and initialize it.
	if (!InitEngine(InApplication))
	{
		SE_LOG(LogEngine, Fatal, L"Could not initialize engine.");
		return -1;
	}

	// Load game module.
	if (!LoadGameModule(gameModule))
	{
		return -1;
	}
	InApplication->SetTitle(GameInstance->GetApplicationName());

	// Start application now!
	InApplication->Idle.AddSObject(this, &SGameEngine::TickEngine);
	GameInstance->Init();
	InApplication->Start();

	Shutdown();
	return 0;
}

SGameInstance* SGameEngine::GetGameInstance()
{
	return GameInstance;
}

SSlateApplication* SGameEngine::GetSlateApplication()
{
	return SlateApplication;
}

void SGameEngine::InitializeSubsystems()
{
	SE_LOG(LogEngine, Verbose, L"Initialize subsystems.");

	std::span Subclasses = Type::FindAllSubclass<SGameEngineSubsystem>();
	SE_LOG(LogEngine, Verbose, L"{} subsystems found.", Subclasses.size() - 1);

	for (auto& Subclass : Subclasses)
	{
		if (Subclass->IsA<SGameEngineSubsystem>())
		{
			continue;
		}

		auto Instance = Cast<SGameEngineSubsystem>(Subclass->Instantiate(this));
		Instance->Init();
		Subsystems.emplace_back(Instance);
	}
}

void SGameEngine::TickEngine(IApplicationInterface::ETickMode ActualTickMode)
{
	using namespace std::chrono;

	auto Tick = TickCalc.DoCalc();

	if (AppTickMode != ActualTickMode || ActualTickMode == IApplicationInterface::ETickMode::Ontime)
	{
		Tick = 0ms;
		AppTickMode = ActualTickMode;
	}

	SystemsTick(Tick);
	GameTick(Tick);
	RenderTick(Tick);
}

void SGameEngine::SystemsTick(std::chrono::duration<float> InDeltaTime)
{
	for (auto& System : Subsystems)
	{
		System->Tick(InDeltaTime.count());
	}
}

void SGameEngine::GameTick(std::chrono::duration<float> InDeltaTime)
{
	SWorld* GameWorld = GetEngineSubsystem<SGameLevelSystem>()->GetGameWorld();
	GameWorld->LevelTick(InDeltaTime.count());
	SlateApplication->TickAndPaint(InDeltaTime.count());
}

void SGameEngine::RenderTick(std::chrono::duration<float> InDeltaTime)
{
	GetEngineSubsystem<SGameRenderSystem>()->ExecuteRenderThread(InDeltaTime.count(), SlateApplication);
}