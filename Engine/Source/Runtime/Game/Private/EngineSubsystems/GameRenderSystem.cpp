// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameRenderSystem.h"

using namespace ::libty;

SGameRenderSystem::SGameRenderSystem()
{
}

void SGameRenderSystem::Inject(SRenderEngine* RenderEngine)
{
	this->RenderEngine = RenderEngine;
	RenderContext = RenderEngine->CreateRenderContext();
}

Task<> SGameRenderSystem::StartAsync(std::stop_token CancellationToken)
{
	return Super::StartAsync(CancellationToken);
}

Task<> SGameRenderSystem::StopAsync(std::stop_token CancellationToken)
{
	return Super::StopAsync(CancellationToken);
}

void SGameRenderSystem::RenderFrame(std::chrono::duration<float> InDeltaTime)
{
}