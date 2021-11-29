// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "SceneRendering/SwapChainRenderTarget.h"
#include "RHI/IRHIFactory.h"
#include "RHI/IRHITexture2D.h"
#include "RHI/IRHIDevice.h"
#include "RHI/IRHIRenderTargetView.h"
#include "RHI/IRHISwapChain.h"
#include "IApplicationInterface.h"

SSwapChainRenderTarget::SSwapChainRenderTarget(IRHIFactory* InFactory, IRHIDevice* InDevice) : Super()
{
	SwapChain = InFactory->CreateSwapChain(InDevice);
	SwapChain->SetOuter(this);
	RTV = InDevice->CreateRenderTargetView(NumBuffers);
	RTV->SetOuter(this);

	const auto [Width, Height] = IApplicationInterface::Get().GetViewportSize();
	ReadyBuffers(Width, Height);
}

void SSwapChainRenderTarget::ResizeBuffers(int32 InWidth, int32 InHeight)
{
	SwapChain->ResizeBuffers(InWidth, InHeight);
	ReadyBuffers(InWidth, InHeight);
}

void SSwapChainRenderTarget::ResolveTarget()
{
	int32 Index = SwapChain->GetCurrentBackBufferIndex();
	InitTarget(RTV, Index, nullptr, 0, ERHIResourceStates::Present);
}

void SSwapChainRenderTarget::Present()
{
	bool bVSync = IApplicationInterface::Get().GetTickMode() == IApplicationInterface::ETickMode::Ontime;
	SwapChain->Present(bVSync ? 1 : 0);
}

void SSwapChainRenderTarget::ReadyBuffers(int32 InWidth, int32 InHeight)
{
	for (int32 i = 0; i < NumBuffers; ++i)
	{
		Buffers[i] = SwapChain->GetBuffer(i);
		RTV->CreateRenderTargetView(i, Buffers[i], nullptr);
	}

	PixelWidth = InWidth;
	PixelHeight = InHeight;
}