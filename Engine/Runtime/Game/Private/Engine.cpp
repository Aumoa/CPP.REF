// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "Application.h"
#include "GameInstance.h"
#include "DeferredGameViewport.h"
#include "RHI/IRHISwapChain.h"
#include "RHI/IRHIResource.h"
#include "Logging/LogMacros.h"
#include "SceneRendering/SceneRenderer.h"
#include "SceneRendering/Scene.h"
#include "RHI/IRHICommandFence.h"
#include "RHI/IRHIImmediateCommandList.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDeferredCommandList.h"
#include "RHI/RHIResourceStates.h"
#include "RHI/RHIResourceGC.h"
#include "RHI/RHIMaterialBundle.h"
#include "Assets/AssetManager.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "PlatformMisc/PlatformInput.h"
#include "PlatformMisc/PlatformImageLoader.h"

#include "D3D12RHI/D3D12DeviceBundle.h"
//#include "VulkanRHI/VulkanDeviceBundle.h"

using namespace std;
using namespace std::chrono;

DEFINE_STATS_GROUP(Engine);

Engine* Engine::gEngine = nullptr;

Engine::Engine() : Super()
	, LogEngine(ELogVerbosity::Verbose, nameof(LogEngine))
	, gameInstance(nullptr)

	, deviceBundle(nullptr)
	, immediateCommandList(nullptr)
	, swapChain(nullptr)
{

}

Engine::~Engine()
{

}

void Engine::Initialize()
{
	if (gEngine != nullptr)
	{
		SE_LOG(LogEngine, Fatal, L"Engine duplication detected.");
		throw Exception("Unexpected exception.");
	}
	gEngine = this;

	PlatformImageLoader::Initialize();
	InitializeBundles();

	autoFence = deviceBundle->CreateCommandFence();
	immediateCommandList = deviceBundle->GetImmediateCommandList();
	swapChain = deviceBundle->GetSwapChain();

	assetManager = NewObject<AssetManager>();
	gameViewport = NewObject<DeferredGameViewport>();
	GApplication.PostSized += bind_delegate(Application_OnPostSized);

	LoadEngineDefaultAssets();

	prev_tick = steady_clock::now();
}

void Engine::PostInitialize()
{
	gameInstance = GApplication.GetGameInstance();
}

void Engine::Tick()
{
	QUICK_SCOPED_CYCLE_COUNTER(Engine, Tick);

	PlatformInput::Tick();

	TickWorld();
	RenderScene();
}

void Engine::Shutdown()
{
	// Waiting for all GPU tasks.
	autoFence->BeginFence();

	CycleStatsGroup::ReadyToShutdown();
	PlatformImageLoader::Shutdown();
}

AssetManager* Engine::GetAssetManager() const
{
	return assetManager.Get();
}

GameViewport* Engine::GetGameViewport() const
{
	return gameViewport.Get();
}

Engine* Engine::GetInstance()
{
	return gEngine;
}

void Engine::CommitBundles(IRHICommandList* inCommandList)
{
	ForEachBundles([inCommandList](auto bundle)
	{
		bundle->Commit(inCommandList);
	});
}

void Engine::TickWorld()
{
	QUICK_SCOPED_CYCLE_COUNTER(Engine, TickWorld);

	auto curr_tick = steady_clock::now();
	Seconds delta = Seconds(curr_tick - prev_tick);
	prev_tick = curr_tick;

	deviceBundle->GetResourceGC()->Collect();
	gameInstance->Tick(delta);
}

void Engine::RenderScene()
{
	World* const world = gameInstance->GetWorld();
	Scene* const scene = world->GetScene();

	immediateCommandList->BeginCommand();
	CommitBundles(immediateCommandList);

	autoFence->BeginFence();

	gameViewport->RenderScene(immediateCommandList, scene);

	{
		QUICK_SCOPED_CYCLE_COUNTER(Engine, RenderFlushing);

		IRHIResource* target = swapChain->GetBuffer(swapChain->CurrentBackBufferIndex);

		immediateCommandList->ResourceTransition(target, ERHIResourceStates::PRESENT, ERHIResourceStates::COPY_DEST);
		immediateCommandList->CopyResource(target, gameViewport->GetRenderTarget());
		immediateCommandList->ResourceTransition(target, ERHIResourceStates::COPY_DEST, ERHIResourceStates::PRESENT);

		immediateCommandList->EndCommand();
		immediateCommandList->Flush();
		swapChain->Present();
	}

	autoFence->EndFence(immediateCommandList);
}

void Engine::InitializeBundles()
{
	auto deviceBundle = NewObject<D3D12DeviceBundle>();
	this->deviceBundle = deviceBundle.Get();
	rhiBundles.emplace_back(move(deviceBundle));

	auto materialBundle = NewObject<RHIMaterialBundle>();
	this->materialBundle = materialBundle.Get();
	rhiBundles.emplace_back(move(materialBundle));

	ForEachBundles([](auto* bundle) { bundle->InitializeBundle(); });
}

void Engine::ForEachBundles(function<void(IRHIBundle*)> action)
{
	for (auto& bundle : rhiBundles)
	{
		action(bundle.Get());
	}
}

void Engine::Application_OnPostSized(int32 x, int32 y)
{
	gameViewport->SetViewportResolution_Internal(x, y);
}