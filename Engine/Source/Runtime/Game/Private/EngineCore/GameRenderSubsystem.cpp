// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "EngineCore/GameRenderSubsystem.h"
#include "RHI/RHIRenderThread.h"
#include "RHI/RHIFactory.h"
#include "RHI/RHIDevice.h"
#include "RHI/RHICommandQueue.h"
#include "RHI/RHIFence.h"
#include "RHI/RHICommandList.h"
#include "RHI/RHIRaytracingPipelineState.h"
#include "RHI/RHIStructures.h"
#include "RHI/RHIResource.h"
#include "Rendering/RaytracingSceneRenderer.h"
#include "GameRenderSubsystem.gen.cpp"

GameRenderSubsystem* GameRenderSubsystem::sInstance = nullptr;

GameRenderSubsystem::GameRenderSubsystem()
{
	check(sInstance == nullptr);
	sInstance = this;
}

GameRenderSubsystem::~GameRenderSubsystem() noexcept
{
	check(sInstance == this);
	sInstance = nullptr;
}

void GameRenderSubsystem::Init()
{
	RenderThread = gcnew RHIRenderThread();

	auto Factory = RHIFactory::CreateFactory();
	Device = Factory->CreateDevice();
	CommandQueue = Device->CreateCommandQueue();
	Fence = Device->CreateFence();
}

void GameRenderSubsystem::Deinit()
{
	Fence.reset();
	CommandQueue.reset();
	Device.reset();

	RenderThread->StopThread();
	RenderThread = nullptr;
}

void GameRenderSubsystem::RegisterSceneView(SceneView* Scene)
{
	check(std::find(Scenes.begin(), Scenes.end(), Scene) == Scenes.end());
	Scenes.emplace_back(Scene);
}

void GameRenderSubsystem::ExecuteRenderTicks(std::function<void()> DisplayWorks)
{
	if (PreviousRenderTask.IsValid())
	{
		// Waiting previous render tick.
#if DO_CHECK
		checkf(PreviousRenderTask.WaitFor(1s), TEXT("Timeout detected on render thread."));
#endif

		PreviousRenderTask.GetResult();
	}

	PreviousRenderTask = RenderThread->ExecuteWorks([this, DisplayWorks, Renderer = RaytracingSceneRenderer()]() mutable
	{
		for (auto& Scene : Scenes)
		{
			Renderer.Render(Scene);
		}

		if (DisplayWorks)
		{
			DisplayWorks();
		}

		CommandQueue->Signal(Fence, ++FenceValue);
		Fence->Wait(FenceValue);
	});
}

void GameRenderSubsystem::JoinRenderThread()
{
	RenderThread->ExecuteWorks(nullptr);
	RenderThread->StopThread();
}

bool GameRenderSubsystem::IsInRenderThread() noexcept
{
	check(sInstance);
	return sInstance->RenderThread->IsInRenderThread();
}