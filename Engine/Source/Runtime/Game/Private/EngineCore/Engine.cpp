// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineCore/Engine.h"
#include "EngineCore/GameRenderSubsystem.h"
#include "Engine.gen.cpp"

Engine::Engine() : Super()
{
}

void Engine::Init()
{
	RenderSystem = gcnew GameRenderSubsystem();
	RenderSystem->Init();
}

void Engine::Deinit()
{
	RenderSystem->Deinit();
	RenderSystem = nullptr;
}

void Engine::Start(GameInstance* Instance)
{
}

void Engine::ExecuteEngineLoop(const TimeSpan& deltaTime)
{
	DispatchEngineTick(deltaTime);
}

void Engine::DispatchEngineTick(const TimeSpan& deltaTime)
{
}