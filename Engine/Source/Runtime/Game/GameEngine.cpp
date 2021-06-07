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
	_deviceContext = CreateSubobject<RHIDeviceContext>(_device);
	_colorShader = CreateSubobject<ColorShader>(_device);
	_colorShader->Compile();
	_rtv = CreateSubobject<RHIRenderTargetView>(_device, 3);

	LogSystem::Log(LogEngine, Info, L"Register engine tick.");
	frameworkView->Idle += [this]() { TickEngine(); };
	frameworkView->Size += [this](int32 width, int32 height) { ResizedApp(width, height); };
}

void GameEngine::TickEngine()
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

	_deviceContext->Begin();
	_deviceContext->OMSetRenderTargets(_rtv, bufferIdx, 1);
	_deviceContext->ClearRenderTargetView(_rtv, bufferIdx, NamedColors::Transparent);
	_deviceContext->RSSetScissorRects(1, &sc);
	_deviceContext->RSSetViewports(1, &vp);
	_deviceContext->SetGraphicsShader(_colorShader);
	_deviceContext->IASetPrimitiveTopology(ERHIPrimitiveTopology::TriangleStrip);
	_deviceContext->DrawInstanced(4, 1);
	_deviceContext->End();

	_primaryQueue->ExecuteDeviceContext(_deviceContext);

	_frameworkViewChain->Present();
	_primaryQueue->Signal();
	_primaryQueue->WaitLastSignal();
}

void GameEngine::ResizedApp(int32 width, int32 height)
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
		RHITexture2D* texture = _frameworkViewChain->GetBuffer(i);
		_rtv->CreateRenderTargetView(texture, i);
	}

	_vpWidth = width;
	_vpHeight = height;
}