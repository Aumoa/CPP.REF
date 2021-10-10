// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "EngineSubsystems/GameRenderSystem.h"
#include "EngineSubsystems/GamePlayerSystem.h"
#include "EngineSubsystems/GameLevelSystem.h"
#include "LogGame.h"
#include "IFrameworkView.h"
#include "GameEngine.h"
#include "Shaders/ColorShader/ColorShader.h"
#include "Shaders/ColorShader/ColorVertexFactory.h"
#include "Shaders/TransparentShader/TransparentShader.h"
#include "Shaders/SlateShader/SlateShader.h"
#include "GameFramework/LocalPlayer.h"
#include "RHI/IRHIFactory.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHISwapChain.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/IRHIDeviceContext.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHIDepthStencilView.h"
#include "Level/World.h"
#include "Scene/Scene.h"
#include "Camera/PlayerCameraManager.h"
#include "GameThreads/RenderThread.h"

DEFINE_LOG_CATEGORY(LogRender);

SGameRenderSystem::SGameRenderSystem() : Super()
{
}

SGameRenderSystem::~SGameRenderSystem()
{
	RenderThread::Shutdown();

	if (_device)
	{
		DestroySubobject(_device);
		_device = nullptr;
	}
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

	SE_LOG(LogRender, Verbose, L"Finish to initialize GameRenderSystem.");
}

void SGameRenderSystem::Present()
{
	RenderThread::WaitForLastWorks();
	RenderThread::ExecuteWorks([this]()
	{
		int32 bufferIdx = _frameworkViewChain->GetCurrentBackBufferIndex();

		RHIViewport vp(0, 0, (float)_vpWidth, (float)_vpHeight, 0, 1.0f);
		RHIScissorRect sc(0, 0, _vpWidth, _vpHeight);

		RHIResourceTransitionBarrier barrierBegin =
		{
			.pResource = _frameworkViewChain->GetBuffer(bufferIdx),
			.StateBefore = ERHIResourceStates::Present,
			.StateAfter = ERHIResourceStates::RenderTarget
		};
		RHIResourceTransitionBarrier barrierEnd =
		{
			.pResource = _frameworkViewChain->GetBuffer(bufferIdx),
			.StateBefore = ERHIResourceStates::RenderTarget,
			.StateAfter = ERHIResourceStates::Present
		};

		_device->BeginFrame();

		_primaryQueue->Begin(0, 0);
		_primaryQueue->ResourceBarrier(barrierBegin);
		_primaryQueue->OMSetRenderTargets(_rtv, bufferIdx, 1, _dsv, 0);
		_primaryQueue->ClearRenderTargetView(_rtv, bufferIdx, NamedColors::Transparent);
		_primaryQueue->ClearDepthStencilView(_dsv, 0, 1.0f, std::nullopt);
		_primaryQueue->RSSetScissorRect(sc);
		_primaryQueue->RSSetViewport(vp);

		SGameLevelSystem* levelSystem = GEngine->GetEngineSubsystem<SGameLevelSystem>();
		SWorld* world = levelSystem->GetWorld();
		APlayerCameraManager* playerCamera = world->GetPlayerCamera();

		SScene* scene = world->GetScene();
		MinimalViewInfo localPlayerView = playerCamera->GetCachedCameraView();
		localPlayerView.AspectRatio = (float)_vpWidth / (float)_vpHeight;
		scene->InitViews(localPlayerView);
		scene->RenderScene(_primaryQueue);

		SLocalPlayer* localPlayer = GEngine->GetEngineSubsystem<SGamePlayerSystem>()->GetLocalPlayer();
		if (localPlayer)
		{
			_primaryQueue->SetGraphicsShader(_slateShader->GetShader());
			_primaryQueue->IASetPrimitiveTopology(ERHIPrimitiveTopology::TriangleStrip);
			localPlayer->Render(_primaryQueue, _slateShader);
		}

		_primaryQueue->ResourceBarrier(barrierEnd);
		_primaryQueue->End();

		_device->EndFrame();
		_frameworkViewChain->Present();
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
		// On the framework view is resized, wait all graphics commands for
		// synchronize and cleanup resource lock states.	{
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
			DestroySubobject(_depthBuffer);
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
	}
}