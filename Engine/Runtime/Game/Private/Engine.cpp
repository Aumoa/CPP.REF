// Copyright 2020 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "D3D12RHI/D3D12Minimal.h"
#include "D3D12RHI/D3D12DeviceBundle.h"
#include "D3D12RHI/D3D12CommandFence.h"
#include "D3D12RHI/D3D12ImmediateCommandList.h"
#include "RHI/IRHISwapChain.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace SC::Runtime::Game::RHI;
using namespace SC::Runtime::Game::D3D12RHI;
using namespace std;

Engine::Engine() : Super()
	, deviceBundle(nullptr)
	, immediateCommandList(nullptr)

	, bPresent(false)
{

}

Engine::~Engine()
{
	autoFence->BeginFence();
}

void Engine::Initialize()
{
	auto deviceBundle = NewObject<D3D12DeviceBundle>();
	this->deviceBundle = deviceBundle.Get();
	rhiBundles.push_back(deviceBundle);

	ForEachBundles([](auto* bundle) { bundle->InitializeBundle(); });

	autoFence = deviceBundle->CreateCommandFence();
	immediateCommandList = deviceBundle->GetImmediateCommandList().Get();
}

void Engine::Tick()
{
	TRefPtr<IRHISwapChain> swapChain = deviceBundle->GetSwapChain();

	if (!bPresent)
	{
		bPresent = swapChain->PresentTest();
	}

	if (bPresent)
	{
		autoFence->BeginFence();

		bPresent = swapChain->Present();

		autoFence->EndFence(immediateCommandList);
	}
}

void Engine::ForEachBundles(function<void(IRHIBundle*)> action)
{
	for (auto& bundle : rhiBundles)
	{
		action(bundle.Get());
	}
}