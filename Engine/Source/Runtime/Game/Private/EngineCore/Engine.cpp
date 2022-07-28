// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineCore/Engine.h"
#include "WorldCore/World.h"
#include "EngineCore/GameRenderSubsystem.h"
#include "Engine.gen.cpp"

Engine::Engine() : Super()
{
}

void Engine::Init()
{
	_world = SpawnGameWorld();

	_renderSystem = gcnew GameRenderSubsystem();
	_renderSystem->Init();
}

void Engine::Deinit()
{
	_renderSystem->Deinit();
	_renderSystem = nullptr;
}

void Engine::ExecuteEngineLoop(const TimeSpan& deltaTime)
{
	DispatchEngineTick(deltaTime);
}

void Engine::DispatchEngineTick(const TimeSpan& deltaTime)
{
	_world->DispatchWorldTick(deltaTime);
}

World* Engine::SpawnGameWorld()
{
	return gcnew World(this);
}