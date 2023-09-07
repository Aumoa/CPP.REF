// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "D3D12RHI/D3D12Viewport.h"

#if PLATFORM_WINDOWS

ND3D12Viewport::ND3D12Viewport(IDXGIFactory7* InFactory, ID3D12CommandQueue* InQueue, HWND hWnd)
{
	RECT ClientRect;
	GetClientRect(hWnd, &ClientRect);

	DXGI_SWAP_CHAIN_DESC1 Desc =
	{
		.Width = (UINT)(ClientRect.right - ClientRect.left),
		.Height = (UINT)(ClientRect.bottom - ClientRect.top),
		.Format = DXGI_FORMAT_R8G8B8A8_UNORM,
		.Stereo = FALSE,
		.SampleDesc = { .Count = 1, .Quality = 0 },
		.BufferUsage = DXGI_USAGE_BACK_BUFFER,
		.BufferCount = 3,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags = 0
	};

	ComPtr<IDXGISwapChain1> SwapChain1;
	HR(InFactory->CreateSwapChainForHwnd(InQueue, hWnd, &Desc, nullptr, nullptr, &SwapChain1));
	HR(SwapChain1.As(&SwapChain4));
}

ND3D12Viewport::~ND3D12Viewport() noexcept
{
}

void ND3D12Viewport::Present()
{
	HR(SwapChain4->Present(0, 0));
}

#endif