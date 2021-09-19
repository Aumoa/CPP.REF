// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
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
#include "Ticking/TickScheduler.h"
#include "GameFramework/LocalPlayer.h"
#include "FreeType/FreeTypeModule.h"
#include "FreeType/FontCachingManager.h"

#include "FreeType/FontFace.h"
#include "Assets/Texture2D.h"

#if _DEBUG
constexpr bool bDebug = true;
#else
constexpr bool bDebug = false;
#endif

DEFINE_LOG_CATEGORY(LogRender);

SGameRenderSystem::SGameRenderSystem() : Super()
{
}

SGameRenderSystem::~SGameRenderSystem()
{
	if (_primaryQueue)
	{
		_primaryQueue->WaitLastSignal();
	}
}

void SGameRenderSystem::Init()
{
	using namespace std;

	SE_LOG(LogRender, Verbose, L"Beginning initialize GameRenderSystem.");

	_scheduler = NewObject<STickScheduler>();
	_device = NewObject<SRHIDevice>(bDebug);
	_primaryQueue = _device->GetPrimaryQueue();
	_deviceContext = NewObject<SRHIDeviceContext>(_device);
	_colorVertexFactory = NewObject<SColorVertexFactory>(_device);
	_colorShader = NewObject<SColorShader>(_device);
	_colorShader->Compile(_colorVertexFactory);
	_rtv = NewObject<SRHIRenderTargetView>(_device, 3);
	_dsv = NewObject<SRHIDepthStencilView>(_device, 1);
	_transparentShader = NewObject<STransparentShader>(_device);
	_transparentShader->Compile(_colorVertexFactory);
	_slateShader = NewObject<SSlateShader>(_device);
	_slateShader->Compile(nullptr);
	_freeTypeModule = NewObject<SFreeTypeModule>();
	_fontCachingManager = _freeTypeModule->CreateCachingMgr(_device);

	SFontFace* ff = _freeTypeModule->CreateFontFace(L"Arial");
	ff->SetFontSize(20);

	_fontCachingManager->StreamGlyphs(ff, L"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ");
	_fontCachingManager->Apply();

	SE_LOG(LogRender, Verbose, L"Register RHI garbage collector.");
	auto gc = [this]()
	{
		Collect();
	};
	_scheduler->AddSchedule({ .Task = gc, .Delay = 10s, .InitDelay = 10s });

	SE_LOG(LogRender, Verbose, L"Finish to initialize GameRenderSystem.");
}

void SGameRenderSystem::Present()
{
	int32 bufferIdx = _frameworkViewChain->GetCurrentBackBufferIndex();

	RHIViewport vp =
	{
		.TopLeftX = 0,
		.TopLeftY = 0,
		.Width = (float)_vpWidth,
		.Height = (float)_vpHeight,
		.MinDepth = 0,
		.MaxDepth = 1.0f
	};

	RHIScissorRect sc =
	{
		.Left = 0,
		.Top = 0,
		.Right = _vpWidth,
		.Bottom = _vpHeight
	};

	RHITransitionBarrier barrierBegin =
	{
		.Resource = _frameworkViewChain->GetBuffer(bufferIdx),
		.StateBefore = ERHIResourceStates::Present,
		.StateAfter = ERHIResourceStates::RenderTarget
	};
	RHITransitionBarrier barrierEnd =
	{
		.Resource = _frameworkViewChain->GetBuffer(bufferIdx),
		.StateBefore = ERHIResourceStates::RenderTarget,
		.StateAfter = ERHIResourceStates::Present
	};

	_deviceContext->Begin();
	_deviceContext->TransitionBarrier(1, &barrierBegin);
	_deviceContext->OMSetRenderTargets(_rtv, bufferIdx, 1, _dsv, 0);
	_deviceContext->ClearRenderTargetView(_rtv, bufferIdx, NamedColors::Transparent);
	_deviceContext->ClearDepthStencilView(_dsv, 0, 1.0f, std::nullopt);
	_deviceContext->RSSetScissorRects(1, &sc);
	_deviceContext->RSSetViewports(1, &vp);

	SGameLevelSystem* levelSystem = GEngine->GetEngineSubsystem<SGameLevelSystem>();
	SWorld* world = levelSystem->GetWorld();
	//APlayerCameraManager* playerCamera = world->GetPlayerCamera();

	//Scene* scene = _gameInstance->GetWorld()->GetScene();
	//MinimalViewInfo localPlayerView = playerCamera->GetCachedCameraView();
	//localPlayerView.AspectRatio = (float)_vpWidth / (float)_vpHeight;
	//scene->InitViews(localPlayerView);
	//scene->RenderScene(_deviceContext);

	SLocalPlayer* localPlayer = GEngine->GetEngineSubsystem<SGamePlayerSystem>()->GetLocalPlayer();
	if (localPlayer)
	{
		_deviceContext->SetGraphicsShader(_slateShader);
		_deviceContext->IASetPrimitiveTopology(ERHIPrimitiveTopology::TriangleStrip);
		localPlayer->Render(_deviceContext, _slateShader);
	}

	_deviceContext->TransitionBarrier(1, &barrierEnd);
	_deviceContext->End();

	_primaryQueue->ExecuteDeviceContext(_deviceContext);

	_frameworkViewChain->Present();
	_primaryQueue->Signal();
	_primaryQueue->WaitLastSignal();
}

void SGameRenderSystem::SetupFrameworkView(IFrameworkView* frameworkView)
{
	_frameworkView = frameworkView;
	_frameworkViewChain = NewObject<SRHISwapChain>(_device, frameworkView, _primaryQueue);
	_frameworkView->Size.AddSObject(this, &SGameRenderSystem::ResizeApp);
}

IFrameworkView* SGameRenderSystem::GetFrameworkView() const
{
	return _frameworkView;
}

void SGameRenderSystem::Collect()
{
	if (int32 collect = _primaryQueue->Collect(); collect)
	{
		SE_LOG(LogRender, Verbose, L"{} garbages are collected.", collect);
	}
}

void SGameRenderSystem::ResizeApp(int32 width, int32 height)
{
	if (width == 0 || height == 0)
	{
		return;
	}

	// On the framework view is resized, wait all graphics commands for
	// synchronize and cleanup resource lock states.
	_primaryQueue->WaitLastSignal();

	_frameworkViewChain->ResizeBuffers(width, height);

	for (int32 i = 0; i < 3; ++i)
	{
		SRHITexture2D* texture = _frameworkViewChain->GetBuffer(i);
		_rtv->CreateRenderTargetView(texture, i);
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

	_depthBuffer = _device->CreateTexture2D(ERHIResourceStates::DepthWrite, ERHIPixelFormat::D24_UNORM_S8_UINT, width, height, clearValue, ERHIResourceFlags::AllowDepthStencil);
	_dsv->CreateDepthStencilView(_depthBuffer, 0);

	_vpWidth = width;
	_vpHeight = height;

	SE_LOG(LogRender, Info, L"Application resized to {}x{}.", width, height);
}