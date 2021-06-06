// Copyright 2020-2021 Aumoa.lib. All right reserved.

import SC.Runtime.Game;
import SC.Runtime.Core;

using enum ELogVerbosity;

GameEngine::GameEngine(bool bDebug) : Super()
	, _bDebug(bDebug)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::InitEngine(GameInstance* gameInstance)
{
	LogSystem::Log(LogEngine, Info, L"Initialize engine.");

	IFrameworkView* frameworkView = gameInstance->GetFrameworkView();

	LogSystem::Log(LogEngine, Info, L"Initialize RHI subsystems.");
	_gameInstance = gameInstance;
	_device = CreateSubobject<RHIDevice>(_bDebug);
	_primaryQueue = CreateSubobject<RHICommandQueue>(_device);
	_frameworkViewChain = CreateSubobject<RHISwapChain>(_device, frameworkView, _primaryQueue);
	_deviceContext = CreateSubobject<RHIDeviceContext>(_device);

	LogSystem::Log(LogEngine, Info, L"Register engine tick.");
	frameworkView->Idle += [this]() { TickEngine(); };
	frameworkView->Size += [this](int32 width, int32 height) { ResizedApp(width, height); };
}

void GameEngine::TickEngine()
{
	_deviceContext->Begin();
	_deviceContext->End();

	_primaryQueue->ExecuteDeviceContext(_deviceContext);

	_frameworkViewChain->Present();
	_primaryQueue->Signal();
	_primaryQueue->WaitLastSignal();
}

void GameEngine::ResizedApp(int32 width, int32 height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	// On the framework view is resized, wait all graphics commands for
	// synchronize and cleanup resource lock states.
	_primaryQueue->WaitLastSignal();

	_frameworkViewChain->ResizeBuffers(width, height);
}