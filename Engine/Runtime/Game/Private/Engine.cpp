// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "Logging/LogMacros.h"
#include "Diagnostics/ScopedCycleCounter.h"
#include "DirectX/DirectXMinimal.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "DirectX/DirectXCommandQueue.h"

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
}

void Engine::Tick()
{
	QUICK_SCOPED_CYCLE_COUNTER(Engine, Tick);
}

void Engine::Shutdown()
{
	CycleStatsGroup::ReadyToShutdown();
}