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

	LogSystem::Log(LogEngine, Info, L"Register engine tick.");
	frameworkView->Idle += [this]() { TickEngine(); };
}

void GameEngine::TickEngine()
{
	_frameworkViewChain->Present();
}