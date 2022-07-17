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

void Engine::ExecuteEngineLoop()
{
	if (_timer.IsRunning())
	{
		// Start tick timer when first executing.
		_timer.Start();
	}

	_timer.Stop();
	float elapsedSeconds = _timer.GetElapsed().GetTotalSeconds<float>();
	_timer.Restart();

	DispatchEngineTick(elapsedSeconds);
}

void Engine::DispatchEngineTick(float elapsedSeconds)
{
	_world->DispatchWorldTick(elapsedSeconds);
}

World* Engine::SpawnGameWorld()
{
	return gcnew World(this);
}