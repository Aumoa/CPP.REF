// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Internal.h"

import SC.Runtime.Core;
import SC.Runtime.RenderCore;

RHISwapChain::RHISwapChain(RHIDevice* device, IWindowView* view, RHICommandQueue* queue) : Super(device)
{
	IDXGIFactory2* dxgi = device->GetFactory();

	DXGI_SWAP_CHAIN_DESC1 chainDesc =
	{
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		.SampleDesc = { 1, 0 },
		.BufferUsage = DXGI_USAGE_BACK_BUFFER,
		.BufferCount = 3,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_IGNORE
	};

	ComPtr<IDXGISwapChain1> swapChain1;
	HWND hWnd = (HWND)view->GetWindowHandle();
	HR(LogRHI, dxgi->CreateSwapChainForHwnd(queue->GetCommandQueue(), hWnd, &chainDesc, nullptr, nullptr, &swapChain1));
	HR(LogRHI, swapChain1.CopyTo(&_swapChain));
}

RHISwapChain::~RHISwapChain()
{
}

void RHISwapChain::Present(uint8 vSyncLevel)
{
	HR(LogRHI, _swapChain->Present((UINT)vSyncLevel, 0));
}