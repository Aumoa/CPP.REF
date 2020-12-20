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

	gEngine = this;
	
	GApplication.PostSized += bind_delegate(Application_OnPostSized);
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

		IRHIRenderTargetView* rtv = basicRTV[swapChain->CurrentBackBufferIndex].Get();
		IRHIResource* target = swapChain->GetBuffer(swapChain->CurrentBackBufferIndex).Get();

		immediateCommandList->BeginCommand();
		immediateCommandList->ResourceTransition(target, RHIResourceStates::PRESENT, RHIResourceStates::RENDER_TARGET);

		immediateCommandList->OMSetRenderTargets(1, &rtv);
		immediateCommandList->ClearRenderTargetView(rtv);

		immediateCommandList->ResourceTransition(target, RHIResourceStates::RENDER_TARGET, RHIResourceStates::PRESENT);
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

void Engine::Application_OnPostSized(int32 width, int32 height)
{
	for (size_t i = 0; i < 3; ++i)
	{
		TRefPtr<IRHIResource> buffer = swapChain->GetBuffer(i);
		basicRTV[i] = deviceBundle->CreateRenderTargetView(buffer.Get());
	}
}