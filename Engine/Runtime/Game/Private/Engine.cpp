// Copyright 2020 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "D3D12RHI/RHIDeviceBundle.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace SC::Runtime::Game::D3D12RHI;

Engine::Engine() : Super()
{

}

Engine::~Engine()
{

}

void Engine::Initialize()
{
	TRefPtr<RHIDeviceBundle> deviceBundle = NewObject<RHIDeviceBundle>();
	deviceBundle->InitializeBundle();
	rhiBundles.push_back(deviceBundle);
}

void Engine::Tick()
{

}