// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "pch.h"
#include "EngineSubsystems/GameRenderSystem.h"
#include "LogGame.h"
#include "IFrameworkView.h"
#include "Shaders/ColorShader/ColorShader.h"
#include "Shaders/ColorShader/ColorVertexFactory.h"
#include "Shaders/TransparentShader/TransparentShader.h"
#include "Shaders/SlateShader/SlateShader.h"
#include "Ticking/TickScheduler.h"

#if _DEBUG
constexpr bool bDebug = true;
#else
constexpr bool bDebug = false;
#endif

DEFINE_LOG_CATEGORY(LogRender);

GameRenderSystem::GameRenderSystem() : Super()
{
}

GameRenderSystem::~GameRenderSystem()
{
}

void GameRenderSystem::Init()
{
	using namespace std;

	Super::Init();

	SE_LOG(LogRender, Verbose, L"Beginning initialize GameRenderSystem.");

	_scheduler = CreateSubobject<TickScheduler>();
	_device = CreateSubobject<RHIDevice>(bDebug);
	_primaryQueue = _device->GetPrimaryQueue();
	_deviceContext = CreateSubobject<RHIDeviceContext>(_device);
	_colorVertexFactory = CreateSubobject<ColorVertexFactory>(_device);
	_colorShader = CreateSubobject<ColorShader>(_device);
	_colorShader->Compile(_colorVertexFactory);
	_rtv = CreateSubobject<RHIRenderTargetView>(_device, 3);
	_dsv = CreateSubobject<RHIDepthStencilView>(_device, 1);
	_transparentShader = CreateSubobject<TransparentShader>(_device);
	_transparentShader->Compile(_colorVertexFactory);
	_slateShader = CreateSubobject<SlateShader>(_device);
	_slateShader->Compile(nullptr);

	SE_LOG(LogRender, Verbose, L"Register RHI garbage collector.");
	auto gc = [this]()
	{
		Collect();
	};
	_scheduler->AddSchedule({ .Task = gc, .Delay = 10s, .InitDelay = 10s });

	SE_LOG(LogRender, Verbose, L"Finish to initialize GameRenderSystem.");
}

void GameRenderSystem::SetupFrameworkView(IFrameworkView* frameworkView)
{
	_frameworkView = frameworkView;
	_frameworkViewChain = CreateSubobject<RHISwapChain>(_device, frameworkView, _primaryQueue);
}

IFrameworkView* GameRenderSystem::GetFrameworkView() const
{
	return _frameworkView;
}

void GameRenderSystem::Collect()
{
	if (int32 collect = _primaryQueue->Collect(); collect)
	{
		SE_LOG(LogRender, Verbose, L"{} garbages are collected.", collect);
	}
}