// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameRenderSystem.h"
#include "EngineSubsystems/GamePlayerSystem.h"
#include "LogGame.h"
#include "IFrameworkView.h"
#include "GameEngine.h"
#include "Shaders/ColorShader/ColorShader.h"
#include "Shaders/ColorShader/ColorVertexFactory.h"
#include "Shaders/TransparentShader/TransparentShader.h"
#include "Shaders/SlateShader/SlateShader.h"
#include "GameFramework/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "RHI/IRHIFactory.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHISwapChain.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/IRHIDeviceContext.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDepthStencilView.h"
#include "Level/World.h"
#include "SceneRendering/Scene.h"
#include "SceneRendering/SceneViewScope.h"
#include "SceneRendering/SceneRenderer.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/MinimalViewInfo.h"
#include "GameThreads/RenderThread.h"
#include "Scene/PrimitiveSceneProxy.h"
#include "Application/SlateApplication.h"
#include "Draw/PaintArgs.h"

DEFINE_LOG_CATEGORY(LogRender);

SGameRenderSystem::SGameRenderSystem() : Super()
{
}

SGameRenderSystem::~SGameRenderSystem()
{
}

void SGameRenderSystem::Init()
{
	using namespace std;

	SE_LOG(LogRender, Verbose, L"Beginning initialize GameRenderSystem.");

	_factory = IRHIFactory::CreateFactory(this);

	// Getting primary adapter for create device.
	IRHIAdapter* adapter = _factory->GetAdapter(0);
	_device = _factory->CreateDevice(adapter);
	_primaryQueue = _device->GetImmediateContext();
	_colorVertexFactory = NewObject<SColorVertexFactory>(_device);
	_colorShader = NewObject<SColorShader>(_device);
	_colorShader->Compile(_colorVertexFactory);
	_rtv = _device->CreateRenderTargetView(3);
	_dsv = _device->CreateDepthStencilView(1);
	_transparentShader = NewObject<STransparentShader>(_device);
	_transparentShader->Compile(_colorVertexFactory);
	_slateShader = NewObject<SSlateShader>(_device);
	_slateShader->Compile(nullptr);

	RenderThread::Init();
	_Scene = NewObject<SScene>(_device);

	SE_LOG(LogRender, Verbose, L"Finish to initialize GameRenderSystem.");
}

void SGameRenderSystem::Deinit()
{
	RenderThread::Shutdown();

	if (_factory && _device)
	{
		_factory->DestroyObject(_device);
		_device = nullptr;
	}
}

void SGameRenderSystem::Tick(float InDeltaTime)
{
}

void SGameRenderSystem::ExecuteRenderThread(float InDeltaTime, SSlateApplication* SlateApp)
{
	SGamePlayerSystem* PlayerSystem = GEngine->GetEngineSubsystem<SGamePlayerSystem>();
	SLocalPlayer* LocalPlayer = PlayerSystem->GetLocalPlayer();

	std::optional<MinimalViewInfo> MinimalPlayerView;
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(); PlayerController != nullptr)
	{
		if (APlayerCameraManager* CameraManager = PlayerController->GetPlayerCameraManager(); CameraManager)
		{
			MinimalPlayerView.emplace(CameraManager->GetCachedCameraView());
		}
	}

	RenderThread::WaitForLastWorks();
	RenderThread::ExecuteWorks([this, MinimalPlayerView = std::move(MinimalPlayerView), InDeltaTime, SlateApp]()
	{
		int32 SwapChainIndex = _frameworkViewChain->GetCurrentBackBufferIndex();
		SceneRenderTarget RenderTarget(_rtv, SwapChainIndex, _dsv, 0, ERHIResourceStates::Present);

		_Scene->BeginScene();
		{
			SceneRenderer Renderer(_Scene);

			if (MinimalPlayerView.has_value())
			{
				float AspectRatio = MinimalPlayerView->AspectRatio;
				if (AspectRatio)
				{
					AspectRatio = RenderTarget.Viewport.Width / RenderTarget.Viewport.Height;
				}

				SceneViewScope PrimarySceneView;
				PrimarySceneView.InitFromCameraView(
					MinimalPlayerView->Location,
					MinimalPlayerView->Rotation,
					MinimalPlayerView->FOVAngle.ToRadians(),
					AspectRatio,
					MinimalPlayerView->NearPlane,
					MinimalPlayerView->FarPlane);

				Renderer.InitViews(std::span(&PrimarySceneView, 1));
			}

			// BEGIN OF FRAME.
			_device->BeginFrame();
			_primaryQueue->Begin(0, 0);

			_Scene->ApplyViewBuffers(_primaryQueue);

			PaintArgs NewArgs(nullptr, InDeltaTime, _primaryQueue, &Renderer);
			SlateApp->PopulateCommandLists(NewArgs);

			// END OF FRAME.
			_primaryQueue->End();
			_frameworkViewChain->Present();

			_device->EndFrame();
		}
		_Scene->EndScene();
	});
}

void SGameRenderSystem::SetupFrameworkView(IFrameworkView* frameworkView)
{
	_frameworkView = frameworkView;
	_frameworkViewChain = _factory->CreateSwapChain(frameworkView, _device);
	_frameworkView->Size.AddSObject(this, &SGameRenderSystem::ResizeApp);
}

IFrameworkView* SGameRenderSystem::GetFrameworkView()
{
	return _frameworkView;
}

void SGameRenderSystem::ResizeApp(int32 width, int32 height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	if (_device)
	{
		RenderThread::WaitForLastWorks();
		RenderThread::ExecuteWorks([this, width, height]()
		{
			// On the framework view is resized, wait all graphics commands for
			// synchronize and cleanup resource lock states.
			_device->BeginFrame();

			_frameworkViewChain->ResizeBuffers(width, height);

			for (int32 i = 0; i < 3; ++i)
			{
				IRHITexture2D* texture = _frameworkViewChain->GetBuffer(i);
				_rtv->CreateRenderTargetView(i, texture, nullptr);
			}

			// Resize depth stencil buffer.
			if (_depthBuffer != nullptr)
			{
				DestroyObject(_depthBuffer);
			}

			RHITexture2DClearValue clearValue =
			{
				.Format = ERHIPixelFormat::D24_UNORM_S8_UINT,
				.DepthStencil = { 1.0f, 0 }
			};

			RHITexture2DDesc dsDesc = {};
			dsDesc.Width = width;
			dsDesc.Height = height;
			dsDesc.DepthOrArraySize = 1;
			dsDesc.MipLevels = 1;
			dsDesc.ClearValue = clearValue;
			dsDesc.Format = ERHIPixelFormat::D24_UNORM_S8_UINT;
			dsDesc.InitialState = ERHIResourceStates::DepthWrite;
			dsDesc.Flags = ERHIResourceFlags::AllowDepthStencil;
			dsDesc.Usage = ERHIBufferUsage::Default;

			_depthBuffer = _device->CreateTexture2D(dsDesc, nullptr);
			_dsv->CreateDepthStencilView(0, _depthBuffer, nullptr);

			_vpWidth = width;
			_vpHeight = height;

			SE_LOG(LogRender, Info, L"Application resized to {}x{}.", width, height);

			_device->EndFrame();
		});
	}
}