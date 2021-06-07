// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "Internal.h"

import SC.Runtime.Core;
import SC.Runtime.RenderCore;

RHISwapChain::RHISwapChain(RHIDevice* device, IWindowView* view, RHICommandQueue* queue) : Super(device)
{
	IDXGIFactory2* dxgi = device->GetFactory();

	DXGI_SWAP_CHAIN_DESC1 chainDesc =
	{
		.Width = 800,
		.Height = 600,
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		.SampleDesc = { 1, 0 },
		.BufferUsage = DXGI_USAGE_BACK_BUFFER,
		.BufferCount = 3,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_IGNORE
	};

	ComPtr<IDXGISwapChain1> swapChain1;
	HWND hWnd = reinterpret_cast<HWND>(view->GetWindowHandle());
	HR(LogRHI, dxgi->CreateSwapChainForHwnd(queue->GetCommandQueue(), hWnd, &chainDesc, nullptr, nullptr, &swapChain1));
	HR(LogRHI, swapChain1.CopyTo(&_swapChain));
}

RHISwapChain::~RHISwapChain()
{
}

void RHISwapChain::Present(uint8 vSyncLevel)
{
	HR_E(LogRHI, _swapChain->Present((UINT)vSyncLevel, 0));
}

void RHISwapChain::ResizeBuffers(int32 width, int32 height)
{
	for (int32 i = 0; i < 3; ++i)
	{
		if (_buffers[i] != nullptr)
		{
			DestroySubobject(_buffers[i]);
			_buffers[i] = nullptr;
		}
	}

	HR_E(LogRHI, _swapChain->ResizeBuffers(0, (UINT)width, (UINT)height, DXGI_FORMAT_UNKNOWN, 0));

	for (int32 i = 0; i < 3; ++i)
	{
		ComPtr<ID3D12Resource> buffer;
		HR(LogRHI, _swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&buffer)));
		_buffers[i] = CreateSubobject<RHITexture2D>(GetDevice(), buffer.Get());
	}
}

RHITexture2D* RHISwapChain::GetBuffer(int32 index) const
{
	return _buffers[index];
}

int32 RHISwapChain::GetCurrentBackBufferIndex() const
{
	return (int32)_swapChain->GetCurrentBackBufferIndex();
}