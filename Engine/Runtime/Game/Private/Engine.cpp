// Copyright 2020 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "D3D12RHI/D3D12Minimal.h"
#include "D3D12RHI/D3D12DeviceBundle.h"
#include "D3D12RHI/D3D12CommandFence.h"
#include "D3D12RHI/D3D12ImmediateCommandList.h"
#include "RHI/IRHISwapChain.h"
#include "Logging/LogVerbosity.h"
#include "Logging/LogMacros.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace SC::Runtime::Game::RHI;
using namespace SC::Runtime::Game::D3D12RHI;
using namespace SC::Runtime::Game::Logging;
using namespace std;

Engine* Engine::gEngine = nullptr;

Engine::Engine() : Super()
	, LogEngine(ELogVerbosity::Verbose, nameof(LogEngine))

	, deviceBundle(nullptr)
	, immediateCommandList(nullptr)
	, swapChain(nullptr)

	, bPresent(false)
{

}

Engine::~Engine()
{
	autoFence->BeginFence();
}

void Engine::Initialize()
{
	if (gEngine != nullptr)
	{
		SE_LOG(LogEngine, Fatal, L"Engine duplication detected.");
		throw HResultException(E_FAIL);
	}

	auto deviceBundle = NewObject<D3D12DeviceBundle>();
	this->deviceBundle = deviceBundle.Get();
	rhiBundles.push_back(deviceBundle);

	ForEachBundles([](auto* bundle) { bundle->InitializeBundle(); });

	autoFence = deviceBundle->CreateCommandFence();
	immediateCommandList = deviceBundle->GetImmediateCommandList().Get();
	swapChain = deviceBundle->GetSwapChain().Get();

	gEngine = this;
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

IRHIDeviceBundle* Engine::DeviceBundle_get() const
{
	return deviceBundle;
}

void Engine::ForEachBundles(function<void(IRHIBundle*)> action)
{
	for (auto& bundle : rhiBundles)
	{
		action(bundle.Get());
	}
}