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
#include "Assets/AssetImporter.h"
#include "GameFramework/LocalPlayer.h"
#include "Misc/CommandLine.h"
#include "PlatformMisc/PlatformModule.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "EngineSubsystems/GameModuleSystem.h"
#include "EngineSubsystems/GameAssetSystem.h"
#include "EngineSubsystems/GameLevelSystem.h"
#include "EngineSubsystems/GamePlayerSystem.h"
#include "EngineSubsystems/GameInputSystem.h"

GameEngine* GEngine = nullptr;

GameEngine::GameEngine() : Super()
{
}

GameEngine::~GameEngine()
{
	for (size_t i = 0; i < _subsystems.size(); ++i)
	{
		// Delete game engine systems without module system
		// for remove module system at last of destructor.
		if (dynamic_cast<GameModuleSystem*>(_subsystems[i]) == nullptr)
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

bool GameEngine::InitEngine()
{
	GEngine = this;
	InitializeSubsystems();
	CoreDelegates::PostEngineInit.Invoke();
	return true;
}

void GameEngine::SetupFrameworkView(IFrameworkView* frameworkView)
{
	GetEngineSubsystem<GameRenderSystem>()->SetupFrameworkView(frameworkView);
	GetEngineSubsystem<GamePlayerSystem>()->SpawnLocalPlayer(frameworkView);

	SE_LOG(LogEngine, Info, L"Register engine tick.");
	frameworkView->Idle += [this]() { TickEngine(); };
}

bool GameEngine::LoadGameModule(std::wstring_view moduleName)
{
	GameModuleSystem* system = GetEngineSubsystem<GameModuleSystem>();
	system->LoadGameModule(moduleName);
	if (_gameInstance = system->LoadGameInstance(); _gameInstance == nullptr)
	{
		SE_LOG(LogEngine, Fatal, L"LoadGameInstance function from {} module return nullptr.", moduleName);
		return false;
	}

	_gameInstance->SetOuter(this);
	_gameInstance->SetWorld(GetEngineSubsystem<GameLevelSystem>()->GetWorld());
	if (!_gameInstance->StartupLevel.IsValid())
	{
		SE_LOG(LogEngine, Fatal, L"GameInstance::StartupLevel is not specified.");
		return false;
	}

	if (!GetEngineSubsystem<GameLevelSystem>()->OpenLevel(_gameInstance->StartupLevel))
	{
		SE_LOG(LogEngine, Fatal, L"Could not startup level.");
		return false;
	}

	return true;
}

int32 GameEngine::InvokedMain(IFrameworkView* frameworkView, std::wstring_view platformArgs)
{
	CoreDelegates::BeginMainInvoked.Invoke();

	CommandLine commandArgs = StringUtils::Split(platformArgs, L" ", true, true);
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
	std::unique_ptr engine = std::make_unique<GameEngine>();
	if (!engine->InitEngine())
	{
		return -1;
	}

	// Load game module.
	if (!engine->LoadGameModule(*moduleName))
	{
		return -1;
	}

	// Setup framework view.
	engine->SetupFrameworkView(frameworkView);

	// Start application now!
	engine->_gameInstance->Init();
	frameworkView->Start();
	return 0;
}

GameInstance* GameEngine::GetGameInstance() const
{
	return _gameInstance;
}

void GameEngine::InitializeSubsystems()
{
	SE_LOG(LogEngine, Verbose, L"Initialize subsystems.");
	_subsystems.emplace_back(NewObject<GameRenderSystem>())->Init();
	_subsystems.emplace_back(NewObject<GameAssetSystem>())->Init();
	_subsystems.emplace_back(NewObject<GameModuleSystem>())->Init();
	_subsystems.emplace_back(NewObject<GameLevelSystem>())->Init();
	_subsystems.emplace_back(NewObject<GamePlayerSystem>())->Init();
	_subsystems.emplace_back(NewObject<GameInputSystem>())->Init();
}

void GameEngine::TickEngine()
{
	using namespace std::chrono;

	auto tick = _tickCalc.DoCalc();
	SystemsTick(tick);
	GameTick(tick);
	RenderTick(tick);
}

void GameEngine::SystemsTick(std::chrono::duration<float> elapsedTime)
{
	for (auto& system : _subsystems)
	{
		system->Tick(elapsedTime);
	}
}

void GameEngine::GameTick(std::chrono::duration<float> elapsedTime)
{
	World* world = GetEngineSubsystem<GameLevelSystem>()->GetWorld();
	world->LevelTick(elapsedTime);
}

void GameEngine::RenderTick(std::chrono::duration<float> elapsedTime)
{
	GetEngineSubsystem<GameRenderSystem>()->Present();
}