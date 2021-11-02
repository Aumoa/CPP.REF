// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameRenderSystem.h"
//#include "Shaders/ColorShader/ColorShader.h"
//#include "Shaders/ColorShader/ColorVertexFactory.h"
//#include "Shaders/TransparentShader/TransparentShader.h"
//#include "Shaders/SlateShader/SlateShader.h"
#include "GameFramework/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "RHI/IRHIFactory.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIDeviceContext.h"
#include "Level/World.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/SceneViewScope.h"
#include "SceneRendering/SwapChainRenderTarget.h"
#include "SceneRendering/SceneRenderContext.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/MinimalViewInfo.h"
#include "Scene/PrimitiveSceneProxy.h"
#include "Application/SlateApplication.h"
//#include "Draw/PaintArgs.h"
//#include "Layout/LayoutImpl.h"
#include "RenderThread.h"
#include "IApplicationInterface.h"

DEFINE_LOG_CATEGORY(LogRender);

SGameRenderSystem::SGameRenderSystem() : Super()
{
}

SGameRenderSystem::~SGameRenderSystem()
{
}

void SGameRenderSystem::Init()
{
	RenderThread::Init();

	Factory = IApplicationInterface::Get().CreateFactory();
	IRHIAdapter* PrimaryAdapter = Factory->GetAdapter(0);
	Device = Factory->CreateDevice(PrimaryAdapter);
	PrimaryQueue = Device->GetImmediateContext();
	SwapChainRT = NewObject<SSwapChainRenderTarget>(Factory, Device);

	IApplicationInterface::Get().Sized.AddSObject(this, &SGameRenderSystem::ResizeApp);
}

void SGameRenderSystem::Deinit()
{
	RenderThread::Shutdown();

	if (Factory && Device)
	{
		Factory->DestroyObject(Device);
		Device = nullptr;
	}
}

void SGameRenderSystem::Tick(float InDeltaTime)
{
}

void SGameRenderSystem::ExecuteRenderThread(float InDeltaTime, SSlateApplication* SlateApp)
{
	RenderThread::WaitForLastWorks();
	RenderThread::ExecuteWorks(PrimaryQueue, [this, InDeltaTime, SlateApp]()
	{
		SwapChainRT->ResolveTarget();

		// BEGIN OF FRAME.
		Device->BeginFrame();
		PrimaryQueue->Begin();

		SceneRenderContext RenderContext(PrimaryQueue, SwapChainRT);
		SlateApp->PopulateCommandLists(RenderContext);

		// END OF FRAME.
		PrimaryQueue->End();
		SwapChainRT->Present();

		Device->EndFrame();
	});
}

IRHIDevice* SGameRenderSystem::GetRHIDevice()
{
	return Device;
}

void SGameRenderSystem::ResizeApp(Vector2N Size)
{
	if (Size.Size() == 0)
	{
		return;
	}

	if (Device)
	{
		RenderThread::WaitForLastWorks();
		RenderThread::ExecuteWorks(PrimaryQueue, [this, Size]()
		{
			// On the framework view is resized, wait all graphics commands for
			// synchronize and cleanup resource lock states.
			Device->BeginFrame();

			SwapChainRT->ResizeBuffers(Size.X, Size.Y);
			SE_LOG(LogRender, Info, L"Application resized to {}x{}.", Size.X, Size.Y);

			Device->EndFrame();
		});
	}
}