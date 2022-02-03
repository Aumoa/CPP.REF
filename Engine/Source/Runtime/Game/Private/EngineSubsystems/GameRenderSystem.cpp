// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameRenderSystem.h"
#include "GameFramework/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "RHI/IRHIFactory.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIDeviceContext.h"
#include "RHI/IRHIDeviceContext2D.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/IRHIBitmap.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDepthStencilView.h"
#include "Level/World.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/SceneViewScope.h"
#include "SceneRendering/SwapChainRenderTarget.h"
#include "SceneRendering/RaytraceSceneRenderer.h"
#include "SceneRenderTarget/ColorRenderTarget.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/MinimalViewInfo.h"
#include "Scene/PrimitiveSceneProxy.h"
#include "Application/SlateApplication.h"
#include "RenderThread.h"
#include "IApplicationInterface.h"
#include "Draw/SlateRenderer.h"

GENERATE_BODY(SGameRenderSystem);
DEFINE_LOG_CATEGORY(LogRender);

SGameRenderSystem::SGameRenderSystem() : Super()
{
	SceneRenderer = gcnew SRaytraceSceneRenderer();
	SlateRenderer = gcnew SSlateRenderer();
}

SGameRenderSystem::~SGameRenderSystem()
{
}

void SGameRenderSystem::Init()
{
	RenderThread.emplace();

	Factory = IApplicationInterface::Get().GetFactory();
	IRHIAdapter* PrimaryAdapter = Factory->GetAdapter(0);
	Device = Factory->CreateDevice(PrimaryAdapter);
	PrimaryQueue = Device->GetImmediateContext();

	SwapChainRT = gcnew SSwapChainRenderTarget(Factory, Device);
	RenderContext = Device->CreateDeviceContext();
	DeviceContext2D = Device->CreateDeviceContext2D();
	ColorRenderTarget = gcnew SColorRenderTarget(Device, IApplicationInterface::Get().GetViewportSize());

	IApplicationInterface::Get().Sized.AddSObject(this, &SGameRenderSystem::ResizeApp);
}

void SGameRenderSystem::Deinit()
{
	RenderThread.reset();

	if (Device)
	{
		Device->Dispose();
		Device = nullptr;
	}

	Super::Deinit();
}

void SGameRenderSystem::Tick(float InDeltaTime)
{
}

void SGameRenderSystem::ExecuteRenderThread(float InDeltaTime, SSlateApplication* SlateApp)
{
	RenderThread::Get()->ExecuteWorks(PrimaryQueue, [this, InDeltaTime, SlateApp](auto)
	{
		SwapChainRT->ResolveTarget();

		// BEGIN OF 3D RENDERING.
		Device->BeginFrame();

		RenderContext->Begin();
		DeviceContext2D->BeginDraw();

		{
			SceneRenderer->BeginDraw(SceneRenderContext(RenderContext, ColorRenderTarget));
			SceneRenderer->EndDraw();
		}

		// END OF 3D RENDERING.
		RenderContext->End();
		PrimaryQueue->ExecuteCommandList(RenderContext);

		{
			DeviceContext2D->SetTarget(ColorRenderTarget->GetRenderBitmap());
			SlateApp->DrawElements(DeviceContext2D, SlateRenderer);
		}

		// Execute 2D commands in ordering.
		DeviceContext2D->EndDraw();
		Device->FlushCommands();

		// Postprocess.
		RHIResourceTransitionBarrier BarrierToCopyDst = {};
		BarrierToCopyDst.pResource = SwapChainRT->RTTexture;
		BarrierToCopyDst.StateBefore = ERHIResourceStates::Present;
		BarrierToCopyDst.StateAfter = ERHIResourceStates::ResolveDest;

		PrimaryQueue->Begin();
		PrimaryQueue->ResourceBarrier(BarrierToCopyDst);
		ColorRenderTarget->CopyRenderTargetOutput(PrimaryQueue, SwapChainRT->RTTexture);
		PrimaryQueue->ResourceBarrier(BarrierToCopyDst.SwapTransition());
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
		RenderThread::Get()->ExecuteWorks(PrimaryQueue, [this, Size](auto)
		{
			// On the framework view is resized, wait all graphics commands for
			// synchronize and cleanup resource lock states.
			Device->BeginFrame();
			Device->FlushCommands();

			SwapChainRT->ResizeBuffers(Size.X, Size.Y);
			ColorRenderTarget->SetViewportSize(Size);
			SE_LOG(LogRender, Info, L"Application resized to {}x{}.", Size.X, Size.Y);

			Device->EndFrame();
		});
	}
}