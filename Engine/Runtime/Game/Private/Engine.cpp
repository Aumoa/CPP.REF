// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "GameInstance.h"
#include "Logging/LogMacros.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "DirectX/DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXCommandQueue.h"
#include "DirectX/DirectXSwapChain.h"

using namespace std;
using namespace std::chrono;

DEFINE_STATS_GROUP(Engine);

Engine::Engine() : Super()
	, gameInstance(nullptr)
{

}

Engine::~Engine()
{

}

void Engine::Initialize(GameInstance* gameInstance)
{
	this->gameInstance = gameInstance;
	deviceBundle = NewObject<DirectXDeviceBundle>();

	DirectXNew(primaryQueue, DirectXCommandQueue, deviceBundle.Get());
	swapChain = NewObject<DirectXSwapChain>(deviceBundle.Get(), primaryQueue.Get(), gameInstance->MainWindow, DXGI_FORMAT_B8G8R8A8_UNORM);
}

void Engine::Tick()
{
	QUICK_SCOPED_CYCLE_COUNTER(Engine, Tick);

	Render();
}

void Engine::Shutdown()
{
	CycleStatsGroup::ReadyToShutdown();
}

void Engine::Render()
{
	swapChain->Present();
}