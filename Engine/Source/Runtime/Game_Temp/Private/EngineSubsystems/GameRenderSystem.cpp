// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameRenderSystem.h"

using namespace ::libty;

SGameRenderSystem::SGameRenderSystem()
{
}

void SGameRenderSystem::Inject(SRenderEngine* RenderEngine, SRenderThread* RenderThread, SSlateApplication* SlateApp)
{
	this->RenderEngine = RenderEngine;
	this->RenderThread = RenderThread;
	this->SlateApp = SlateApp;

	RenderContext = RenderEngine->CreateRenderContext();
	Fence = RenderContext->OwningDevice->CreateFence();
}

Task<> SGameRenderSystem::StartAsync(std::stop_token CancellationToken)
{
	return Super::StartAsync(CancellationToken);
}

Task<> SGameRenderSystem::StopAsync(std::stop_token CancellationToken)
{
	RenderThread->Shutdown();
	return Super::StopAsync(CancellationToken);
}

void SGameRenderSystem::RenderFrame(std::chrono::duration<float> InDeltaTime)
{
	RenderThread->ExecuteWorks(RenderContext, [this](SRenderContext* RContext)
	{
		// Waiting for previous execution.
		while (Fence->GetCompletedValue() < FenceValue)
		{
			Fence->SetEventOnCompletion(FenceValue, 1s).Wait();
		}

		SlateApp->RenderApp(RContext);
		RContext->RenderQueue->Signal(Fence, ++FenceValue);
	});
}