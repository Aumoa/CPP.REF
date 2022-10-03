// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineCore/GameEngine.h"
#include "WorldCore/World.h"
#include "GameFramework/GameInstance.h"
#include "EngineCore/GameRenderSubsystem.h"
#include "Rendering/SceneView.h"
#include "GameEngine.gen.cpp"

GameEngine::GameEngine() : Super()
{
}

GameEngine::~GameEngine() noexcept
{
}

void GameEngine::Init()
{
	Super::Init();
	GameWorld = SpawnGameWorld();
}

void GameEngine::Start(GameInstance* Instance)
{
	Super::Start(Instance);
	Instance->Start(GameWorld);
}

void GameEngine::DestroyGameWorld()
{
	if (GameWorld)
	{
		GameWorld->DestroyWorld();
		GameWorld = nullptr;
	}
}

void GameEngine::DispatchEngineTick(const TimeSpan& InDeltaTime)
{
	Super::DispatchEngineTick(InDeltaTime);
	GameWorld->DispatchWorldTick(InDeltaTime);
}

World* GameEngine::SpawnGameWorld()
{
	auto* Instance = gcnew World();
	Instance->Rename(this, TEXT("World"));

	auto* Subsystem = GetEngineSubsystem<GameRenderSubsystem>();
	GameViewport = std::make_unique<SceneView>(Instance->GetScene());
	Subsystem->RegisterSceneView(GameViewport.get());

	return Instance;
}