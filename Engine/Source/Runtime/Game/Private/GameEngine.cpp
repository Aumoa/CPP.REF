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

GENERATE_BODY(SGameEngine)

DECLARE_STAT_GROUP("Engine", STATGROUP_Engine);
DECLARE_CYCLE_STAT("Tick", STAT_Tick, STATGROUP_Engine);
DECLARE_CYCLE_STAT("  SystemTick", STAT_SystemTick, STATGROUP_Engine);
DECLARE_CYCLE_STAT("  GameTick", STAT_GameTick, STATGROUP_Engine);
DECLARE_CYCLE_STAT("  ExecuteRenderThread", STAT_ExecuteRenderThread, STATGROUP_Engine);

SGameEngine* GEngine = nullptr;

SGameEngine::SGameEngine() : Super()
{
}

bool SGameEngine::InitEngine(IApplicationInterface* InApplication)
{
	GEngine = this;

	// CollectGarbageEveryFrame = false
	GC.SetFlushInterval(60);

	SlateApplication = CreateSlateApplication();
	SlateApplication->Init(InApplication);

	InitializeSubsystems();
	CoreDelegates::PostEngineInit.Broadcast();

	Thread::GetCurrentThread()->SetFriendlyName(L"[Game Thread]");
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

	for (auto& System : Subsystems)
	{
		System->Deinit();
	}

	GC.SuppressFinalize(GameInstance);
	GC.SuppressFinalize(SlateApplication);
	GC.Collect(true);

	Subsystems.clear();
	SubsystemView.clear();
}

int32 SGameEngine::GuardedMain(IApplicationInterface* InApplication, std::wstring_view gameModule)
{
	CoreDelegates::BeginMainInvoked.Broadcast();
	GC.Collect();

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
	InApplication->SetTitle(ANSI_TO_WCHAR(SE_APPLICATION));

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

SSlateApplication* SGameEngine::CreateSlateApplication()
{
	return gcnew SSlateApplication();
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

		auto Instance = Cast<SGameEngineSubsystem>(Subclass->Instantiate());
		Instance->Init();
		Subsystems.emplace_back(Instance);

		Type* Class = Instance->GetType();
		while (Class)
		{
			auto [It, bStatus] = SubsystemView.emplace(Class->GetHashCode(), Instance);
			if (bStatus)
			{
				break;
			}

			Class = Class->GetSuper();
		}
	}

	for (auto& Subsystem : Subsystems)
	{
		Subsystem->PostInit();
	}
}

void SGameEngine::TickEngine(IApplicationInterface::ETickMode ActualTickMode)
{
	using namespace std::chrono;

	SCOPE_CYCLE_COUNTER(STAT_Tick);
	auto Tick = TickCalc.DoCalc();

	if (AppTickMode != ActualTickMode || ActualTickMode == IApplicationInterface::ETickMode::Ontime)
	{
		Tick = 0ms;
		AppTickMode = ActualTickMode;
	}

	GC.Hint();

	SystemsTick(Tick);
	GameTick(Tick);
	RenderTick(Tick);
}

void SGameEngine::SystemsTick(std::chrono::duration<float> InDeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_SystemTick);

	for (auto& System : Subsystems)
	{
		System->Tick(InDeltaTime.count());
	}
}

void SGameEngine::GameTick(std::chrono::duration<float> InDeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_GameTick);

	SWorld* GameWorld = GetEngineSubsystem<SGameLevelSystem>()->GetGameWorld();

	// Flush all deferred task with this frame.
	Task<>::FlushDeferred();

	// Execute level tick.
	GameWorld->LevelTick(InDeltaTime.count());

	// Tick and paint.
	SlateApplication->TickAndPaint(InDeltaTime.count());
}

void SGameEngine::RenderTick(std::chrono::duration<float> InDeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_ExecuteRenderThread);

	GetEngineSubsystem<SGameRenderSystem>()->ExecuteRenderThread(InDeltaTime.count(), SlateApplication);
}
