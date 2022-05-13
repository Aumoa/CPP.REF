// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "GameEngine.h"
#include "LogGame.h"
#include "GameInstance.h"
#include "CoreDelegates.h"
#include "RenderThread.h"
#include "Threading/Thread.h"
#include "Level/World.h"
#include "EngineSubsystems/GameEngineSubsystem.h"
#include "EngineSubsystems/GameModuleSystem.h"
#include "EngineSubsystems/GameLevelSystem.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "Misc/AutoConsoleVariable.h"

DECLARE_STAT_GROUP("Engine", STATGROUP_Engine);
DECLARE_CYCLE_STAT("Tick", STAT_Tick, STATGROUP_Engine);
DECLARE_CYCLE_STAT("  SystemTick", STAT_SystemTick, STATGROUP_Engine);
DECLARE_CYCLE_STAT("  GameTick", STAT_GameTick, STATGROUP_Engine);
DECLARE_CYCLE_STAT("  ExecuteRenderThread", STAT_ExecuteRenderThread, STATGROUP_Engine);

using namespace ::libty;

SGameEngine* ::libty::Game::GEngine = nullptr;

SGameEngine::SGameEngine() : Super()
{
}

bool SGameEngine::InitEngine(IApplicationInterface* InApplication)
{
	GEngine = this;
	Thread::GetCurrentThread()->SetFriendlyName(TEXT("[Game Thread]"));

	InitializeSubsystems();
	CoreDelegates::PostEngineInit.Broadcast();

	return true;
}

bool SGameEngine::LoadGameModule(StringView InModuleName)
{
	SGameModuleSystem* ModuleSystem = GetEngineSubsystem<SGameModuleSystem>();
	ModuleSystem->LoadGameModule(InModuleName);
	if (GameInstance = ModuleSystem->LoadGameInstance(); GameInstance == nullptr)
	{
		throw Exception(String::Format(TEXT("LoadGameInstance function from {} module return nullptr."), InModuleName));
	}

	SWorld* GameWorld = GetEngineSubsystem<SGameLevelSystem>()->GetGameWorld();
	GameInstance->SetOuter(GameWorld);
	if (!GameInstance->StartupLevel.IsValid())
	{
		throw Exception(TEXT("SGameInstance::StartupLevel is not specified."));
	}

	if (!GetEngineSubsystem<SGameLevelSystem>()->OpenLevel(GameInstance->StartupLevel))
	{
		throw Exception(TEXT("Could not startup level."));
	}

	return true;
}

void SGameEngine::Shutdown()
{
	SubsystemCollection->StopAsync().GetResult();

	GC->SuppressFinalize(GameInstance);
	GC->Collect(true);

	Subsystems.clear();
	SubsystemView.clear();
}

namespace AutoConsoleVars
{
	namespace GC
	{
		AutoConsoleVariable<float> CollectInterval(TEXT("GC.CollectInterval"), 60.0f);
	}
}

int32 SGameEngine::GuardedMain(IApplicationInterface* InApplication, StringView gameModule)
{
	CoreDelegates::BeginMainInvoked.Broadcast();
	GC->Collect();

	GC->SetFlushInterval(AutoConsoleVars::GC::CollectInterval.GetValue());
	AutoConsoleVars::GC::CollectInterval.VariableCommitted.AddRaw([](AutoConsoleVariable<float>& V)
	{
		GC->SetFlushInterval(V.GetValue());
	});

	// Create GameEngine instance and initialize it.
	if (!InitEngine(InApplication))
	{
		throw Exception("Could not initialize engine.");
	}

	// Load game module.
	if (!LoadGameModule(gameModule))
	{
		return -1;
	}
	InApplication->SetTitle(String::AsUnicode(SE_APPLICATION));

	// Start application now!
	InApplication->Idle.AddObject(this, &SGameEngine::TickEngine);
	GameInstance->Init();
	InApplication->Start();

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

	SObjectFactoryBuilder* Builder = SObjectFactory::CreateBuilder();

	Builder->AddSingleton<SRenderEngine>();
	Builder->AddSingleton<SSlateCollection>();

	auto Subclasses = SType::GetDerivedTypes(typeof(SGameEngineSubsystem));
	SE_LOG(LogEngine, Verbose, L"{} subsystems found.", Subclasses.size() - 1);

	for (auto& Subclass : Subclasses)
	{
		if (Subclass->IsA<SGameEngineSubsystem>())
		{
			continue;
		}

		Builder->AddHostedService(Subclass);
	}

	SubsystemCollection = Cast<SObjectFactory>(Builder->Build());
	SubsystemCollection->StartAsync().GetResult();
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

	GC->Hint();

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

	// Execute level tick.
	GameWorld->LevelTick(InDeltaTime.count());

	// Tick and paint.
	//SlateApplication->TickAndPaint(InDeltaTime.count());
}

void SGameEngine::RenderTick(std::chrono::duration<float> InDeltaTime)
{
	SCOPE_CYCLE_COUNTER(STAT_ExecuteRenderThread);

	GetEngineSubsystem<SGameRenderSystem>()->RenderFrame(InDeltaTime);
}
