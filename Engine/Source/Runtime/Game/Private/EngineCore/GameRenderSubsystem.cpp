// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineCore/GameRenderSubsystem.h"
#include "RHI/RHIRenderThread.h"
#include "RHI/RHIFactory.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHICommandQueue.h"
#include "Diagnostics/PerformanceTimer.h"
#include "GameRenderSubsystem.gen.cpp"

GameRenderSubsystem::GameRenderSubsystem()
{
}

GameRenderSubsystem::~GameRenderSubsystem() noexcept
{
}

void GameRenderSubsystem::Init()
{
	_renderThread = gcnew RHIRenderThread();
	auto factory = RHIFactory::CreateFactory();
	_device = factory->CreateDevice();
	_commandQueue = _device->CreateCommandQueue();
}

void GameRenderSubsystem::Deinit()
{
	_commandQueue.reset();
	_device.reset();

	_renderThread->StopThread();
	_renderThread = nullptr;
}

void GameRenderSubsystem::ExecuteRenderTicks()
{
	if (_previousRenderTick.IsValid())
	{
		// Waiting previous render tick.
#if DO_CHECK
		Task<> timeoutResult = Task<>::WhenAny(_previousRenderTick, Task<>::Delay(1000ms)).GetResult();
		checkf(timeoutResult == _previousRenderTick, TEXT("RenderThread Deadlock Detected."));
#else
		_previousRenderTick.GetResult();
#endif
	}

	_previousRenderTick = _renderThread->ExecuteWorks([]()
	{
	});
}