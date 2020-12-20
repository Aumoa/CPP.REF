// Copyright 2020 Aumoa.lib. All right reserved.

#include "Engine.h"

#include "Application.h"
#include "D3D12RHI/D3D12Minimal.h"
#include "D3D12RHI/D3D12DeviceBundle.h"
#include "D3D12RHI/D3D12CommandFence.h"
#include "D3D12RHI/D3D12ImmediateCommandList.h"
#include "D3D12RHI/D3D12RenderTargetView.h"
#include "RHI/IRHISwapChain.h"
#include "RHI/IRHIResource.h"
#include "Logging/LogVerbosity.h"
#include "Logging/LogMacros.h"
#include "SceneRendering/SceneRenderer.h"

using namespace SC::Runtime::Core;
using namespace SC::Runtime::Game;
using namespace SC::Runtime::Game::RHI;
using namespace SC::Runtime::Game::D3D12RHI;
using namespace SC::Runtime::Game::Logging;
using namespace SC::Runtime::Game::SceneRendering;
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
	// Waiting for all GPU tasks.
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

	sceneRenderer = NewObject<SceneRenderer>(this->deviceBundle);

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

		IRHIResource* target = swapChain->GetBuffer(swapChain->CurrentBackBufferIndex).Get();

		immediateCommandList->BeginCommand();
		//immediateCommandList->ResourceTransition(target, RHIResourceStates::PRESENT, RHIResourceStates::COPY_DEST);

		// TODO: SceneRenderer
		sceneRenderer->BeginRender();
		sceneRenderer->EndRender();

		//immediateCommandList->ResourceTransition(target, RHIResourceStates::COPY_DEST, RHIResourceStates::PRESENT);
		immediateCommandList->EndCommand();
		immediateCommandList->Flush();

		bPresent = swapChain->Present();

		autoFence->EndFence(immediateCommandList);
	}
}

IRHIDeviceBundle* Engine::DeviceBundle_get() const
{
	return deviceBundle;
}

Engine* Engine::GetInstance()
{
	return gEngine;
}

void Engine::ForEachBundles(function<void(IRHIBundle*)> action)
{
	for (auto& bundle : rhiBundles)
	{
		action(bundle.Get());
	}
}