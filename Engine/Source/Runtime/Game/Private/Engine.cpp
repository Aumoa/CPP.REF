// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "GameInstance.h"
#include "DeferredGameViewport.h"
#include "World.h"
#include "Logging/LogMacros.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "Windows/CoreWindow.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXCommandQueue.h"
#include "DirectX/DirectXSwapChain.h"
#include "DirectX/DirectXImmediateContext.h"
#include "DirectX/DirectXAutoFence.h"
#include "DirectX/DirectXCompatibleRenderTarget.h"
#include "Assets/AssetManager.h"
#include "Assets/CachedShaderLibrary.h"
#include "Assets/AssetImporter.h"
#include "Time/StepTimer.h"
#include "COM/COMDeviceBundle.h"

using namespace std;
using namespace std::chrono;

DEFINE_STATS_GROUP(Engine);

Engine::Engine() : Super()
	, gameInstance(nullptr)
{
	tickTimer = NewObject<StepTimer>();
}

Engine::~Engine()
{

}

void Engine::Initialize(GameInstance* gameInstance)
{
	this->gameInstance = gameInstance;
	deviceBundle = NewObject<DirectXDeviceBundle>();
	DirectXNew(primaryQueue, DirectXCommandQueue, deviceBundle.Get());
	assetManager = NewObject<AssetManager>(this);
	comBundle = NewObject<COMDeviceBundle>();
	assimp = NewObject<AssetImporter>(this);

	swapChain = NewObject<DirectXSwapChain>(deviceBundle.Get(), primaryQueue.Get(), gameInstance->MainWindow, DXGI_FORMAT_B8G8R8A8_UNORM);
	DirectXNew(immediateContext, DirectXImmediateContext, deviceBundle.Get(), primaryQueue.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
	DirectXNew(immediateFence, DirectXAutoFence, deviceBundle.Get());
	gameViewport = NewObject<DeferredGameViewport>(deviceBundle.Get());
	shaderLibrary = NewObject<CachedShaderLibrary>(deviceBundle.Get());

	gameInstance->MainWindow->Sizing += bind_delegate(MainWindow_OnSizing);
}

void Engine::Tick()
{
	CycleStatsGroup::ResolveFrameDiagnostics();

	QUICK_SCOPED_CYCLE_COUNTER(Engine, Tick);

	tickTimer->Tick();
	gameInstance->TickWorld(tickTimer->ElapsedSeconds);
	Render();
	primaryQueue->CollectPendingReferences();
}

void Engine::Shutdown()
{
	immediateFence->Wait();

	CycleStatsGroup::ReadyToShutdown();
	swapChain.Reset();
	assimp.Reset();
}

void Engine::Render()
{
	QUICK_SCOPED_CYCLE_COUNTER(Engine, Render);

	immediateFence->Wait();
	immediateContext->BeginDraw();
	{
		ID3D12GraphicsCommandList4* commandList = immediateContext->GetCommandList();

		Scene* scene = gameInstance->GetWorld()->GetScene();
		gameViewport->RenderScene(immediateContext.Get(), scene);

		ID3D12Resource* src = gameViewport->GetCompatibleRenderTarget()->GetResource();
		ID3D12Resource* dst = swapChain->GetCurrentBuffer();

		D3D12_RESOURCE_BARRIER barrier = { };
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = dst;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;

		commandList->ResourceBarrier(1, &barrier);
		commandList->CopyResource(dst, src);

		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		commandList->ResourceBarrier(1, &barrier);
	}
	immediateContext->EndDraw();
	swapChain->Present();

	immediateFence->AcceptSignal(primaryQueue.Get());
}

DirectXDeviceBundle* Engine::GetDeviceBundle() const
{
	return deviceBundle.Get();
}

DirectXCommandQueue* Engine::GetPrimaryCommandQueue() const
{
	return primaryQueue.Get();
}

AssetManager* Engine::GetAssetManager() const
{
	return assetManager.Get();
}

CachedShaderLibrary* Engine::GetCachedShaderLibrary() const
{
	return shaderLibrary.Get();
}

GameViewport* Engine::GetLocalViewport() const
{
	return gameViewport.Get();
}

AssetImporter* Engine::GetAssetImporter() const
{
	return assimp.Get();
}

COMDeviceBundle* Engine::GetCOMDevice() const
{
	return comBundle.Get();
}

void Engine::MainWindow_OnSizing(int32 x, int32 y)
{
	if (x * y != 0)
	{
		immediateFence->Wait();

		swapChain->ResizeBuffers(x, y);
		gameViewport->SetResolution(x, y);
		SE_LOG(LogEngine, Verbose, L"SwapChain was resize to [{0}x{1}].", x, y);
	}
	else
	{
		SE_LOG(LogEngine, Verbose, L"Window size is nearly zero. Skip resizing swap chain.");
	}
}