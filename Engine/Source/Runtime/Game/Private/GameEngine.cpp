// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "GameEngine.h"
#include "LogGame.h"
#include "GameInstance.h"
#include "IFrameworkView.h"
#include "CoreDelegates.h"
#include "Level/World.h"
#include "Camera/PlayerCameraManager.h"
#include "GameFramework/LocalPlayer.h"
#include "PlatformMisc/PlatformModule.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "EngineSubsystems/GameModuleSystem.h"
#include "EngineSubsystems/GameAssetSystem.h"
#include "EngineSubsystems/GameLevelSystem.h"
#include "EngineSubsystems/GamePlayerSystem.h"
#include "EngineSubsystems/GameInputSystem.h"
#include "Threading/Thread.h"

SGameEngine* GEngine = nullptr;

SGameEngine::SGameEngine() : Super()
{
}

SGameEngine::~SGameEngine()
{
	for (size_t i = 0; i < _Subsystems.size(); ++i)
	{
		// Delete game engine systems without module system
		// for remove module system at last of destructor.
		if (dynamic_cast<SGameModuleSystem*>(_Subsystems[i]) == nullptr)
		{
			DestroySubobject(_Subsystems[i]);
		}
	}
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
	GetEngineSubsystem<SGamePlayerSystem>()->SpawnLocalPlayer(frameworkView);

	SE_LOG(LogEngine, Info, L"Register engine tick.");
	frameworkView->Idle.AddSObject(this, &SGameEngine::TickEngine);

	frameworkView->SetFrameworkTitle(_GameInstance->GetApplicationName());
}

bool SGameEngine::LoadGameModule(std::wstring_view moduleName)
{
	SGameModuleSystem* system = GetEngineSubsystem<SGameModuleSystem>();
	system->LoadGameModule(moduleName);
	if (_GameInstance = system->LoadGameInstance(); _GameInstance == nullptr)
	{
		SE_LOG(LogEngine, Fatal, L"LoadGameInstance function from {} module return nullptr.", moduleName);
		return false;
	}

	SWorld* GameWorld = GetEngineSubsystem<SGameLevelSystem>()->GetGameWorld();
	_GameInstance->SetOuter(GameWorld);
	if (!_GameInstance->StartupLevel.IsValid())
	{
		SE_LOG(LogEngine, Fatal, L"SGameInstance::StartupLevel is not specified.");
		return false;
	}

	if (!GetEngineSubsystem<SGameLevelSystem>()->OpenLevel(_GameInstance->StartupLevel))
	{
		SE_LOG(LogEngine, Fatal, L"Could not startup level.");
		return false;
	}

	return true;
}

void SGameEngine::Shutdown()
{
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

	// Load game module.
	if (!LoadGameModule(gameModule))
	{
		return -1;
	}

	// Setup framework view.
	SetupFrameworkView(frameworkView);

	// Start application now!
	_GameInstance->Init();
	frameworkView->Start();

	Shutdown();
	return 0;
}

SGameInstance* SGameEngine::GetGameInstance()
{
	return _GameInstance;
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

	auto tick = _TickCalc.DoCalc();
	SystemsTick(tick);
	GameTick(tick);
	RenderTick(tick);

	static int32 counter = 0;
	if (++counter >= 100)
	{
		SE_LOG(LogEngine, Verbose, L"Avg: {}, FPS: {}", _TickCalc.GetAverageSeconds(), _TickCalc.GetAverageFPS());
		counter = 0;
	}
}

void SGameEngine::SystemsTick(std::chrono::duration<float> elapsedTime)
{
	for (auto& system : _Subsystems)
	{
		system->Tick(elapsedTime);
	}
}

void SGameEngine::GameTick(std::chrono::duration<float> elapsedTime)
{
	SWorld* GameWorld = GetEngineSubsystem<SGameLevelSystem>()->GetGameWorld();
	GameWorld->LevelTick(elapsedTime);
}

void SGameEngine::RenderTick(std::chrono::duration<float> elapsedTime)
{
	GetEngineSubsystem<SGameRenderSystem>()->ExecuteRenderThread();
}