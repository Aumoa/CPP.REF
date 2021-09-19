// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "GameEngine.h"
#include "LogGame.h"
#include "GameInstance.h"
#include "IFrameworkView.h"
#include "CoreDelegates.h"
#include "Level/World.h"
#include "Camera/PlayerCameraManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneRenderer.h"
#include "GameFramework/LocalPlayer.h"
#include "Misc/CommandLine.h"
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
	for (size_t i = 0; i < _subsystems.size(); ++i)
	{
		// Delete game engine systems without module system
		// for remove module system at last of destructor.
		if (dynamic_cast<SGameModuleSystem*>(_subsystems[i]) == nullptr)
		{
			DestroySubobject(_subsystems[i]);
		}
	}

	// Remove game instance.
	if (_gameInstance)
	{
		DestroySubobject(_gameInstance);
		_gameInstance = nullptr;
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
	frameworkView->Idle += [this]() { TickEngine(); };

	frameworkView->SetFrameworkTitle(_gameInstance->GetApplicationName());
}

bool SGameEngine::LoadGameModule(std::wstring_view moduleName)
{
	SGameModuleSystem* system = GetEngineSubsystem<SGameModuleSystem>();
	system->LoadGameModule(moduleName);
	if (_gameInstance = system->LoadGameInstance(); _gameInstance == nullptr)
	{
		SE_LOG(LogEngine, Fatal, L"LoadGameInstance function from {} module return nullptr.", moduleName);
		return false;
	}

	_gameInstance->SetOuter(this);
	_gameInstance->SetWorld(GetEngineSubsystem<SGameLevelSystem>()->GetWorld());
	if (!_gameInstance->StartupLevel.IsValid())
	{
		SE_LOG(LogEngine, Fatal, L"SGameInstance::StartupLevel is not specified.");
		return false;
	}

	if (!GetEngineSubsystem<SGameLevelSystem>()->OpenLevel(_gameInstance->StartupLevel))
	{
		SE_LOG(LogEngine, Fatal, L"Could not startup level.");
		return false;
	}

	return true;
}

int32 SGameEngine::InvokedMain(IFrameworkView* frameworkView, std::wstring_view platformArgs)
{
	CoreDelegates::BeginMainInvoked.Invoke();

	SCommandLine commandArgs = StringUtils::Split(platformArgs, L" ", true, true);
	size_t gameModuleIdx = commandArgs.GetArgument(L"--GameDll");
	if (gameModuleIdx == -1)
	{
		SE_LOG(LogEngine, Fatal, L"GameModule does not specified.");
		return -1;
	}

	std::optional<std::wstring_view> moduleName = commandArgs.GetArgument(gameModuleIdx + 1);
	if (!moduleName)
	{
		SE_LOG(LogEngine, Fatal, L"GameModule does not specified.");
		return -1;
	}

	// Create GameEngine instance and initialize it.
	if (!InitEngine())
	{
		SE_LOG(LogEngine, Fatal, L"Could not initialize engine.");
		return -1;
	}

	// Load game module.
	if (!LoadGameModule(*moduleName))
	{
		return -1;
	}

	// Setup framework view.
	SetupFrameworkView(frameworkView);

	// Start application now!
	_gameInstance->Init();
	frameworkView->Start();
	return 0;
}

SGameInstance* SGameEngine::GetGameInstance() const
{
	return _gameInstance;
}

void SGameEngine::InitializeSubsystems()
{
	SE_LOG(LogEngine, Verbose, L"Initialize subsystems.");
	_subsystems.emplace_back(NewObject<SGameRenderSystem>())->Init();
	_subsystems.emplace_back(NewObject<SGameAssetSystem>())->Init();
	_subsystems.emplace_back(NewObject<SGameModuleSystem>())->Init();
	_subsystems.emplace_back(NewObject<SGameLevelSystem>())->Init();
	_subsystems.emplace_back(NewObject<SGamePlayerSystem>())->Init();
	_subsystems.emplace_back(NewObject<SGameInputSystem>())->Init();
}

void SGameEngine::TickEngine()
{
	using namespace std::chrono;

	auto tick = _tickCalc.DoCalc();
	SystemsTick(tick);
	GameTick(tick);
	RenderTick(tick);
}

void SGameEngine::SystemsTick(std::chrono::duration<float> elapsedTime)
{
	for (auto& system : _subsystems)
	{
		system->Tick(elapsedTime);
	}
}

void SGameEngine::GameTick(std::chrono::duration<float> elapsedTime)
{
	World* world = GetEngineSubsystem<SGameLevelSystem>()->GetWorld();
	world->LevelTick(elapsedTime);
}

void SGameEngine::RenderTick(std::chrono::duration<float> elapsedTime)
{
	GetEngineSubsystem<SGameRenderSystem>()->Present();
}