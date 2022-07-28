// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineCore/GameRenderSubsystem.h"
#include "RHI/RHIRenderThread.h"
#include "RHI/RHIFactory.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHICommandQueue.h"
#include "RHI/RHIFence.h"
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
	_fence = _device->CreateFence();
}

void GameRenderSubsystem::Deinit()
{
	_fence.reset();
	_commandQueue.reset();
	_device.reset();

	_renderThread->StopThread();
	_renderThread = nullptr;
}

void GameRenderSubsystem::ExecuteRenderTicks(std::function<void()> presentWorks)
{
	if (_previousRenderTick.IsValid())
	{
		// Waiting previous render tick.
#if DO_CHECK
		checkf(_previousRenderTick.WaitFor(1s), TEXT("Timeout detected on render thread."));
#endif

		_previousRenderTick.GetResult();
	}

	_previousRenderTick = _renderThread->ExecuteWorks([this, presentWorks]()
	{
		if (presentWorks)
		{
			presentWorks();
		}

		_commandQueue->Signal(_fence, ++_fenceValue);
		_fence->Wait(_fenceValue);
	});
}

void GameRenderSubsystem::JoinRenderThread()
{
	_renderThread->StopThread();
}