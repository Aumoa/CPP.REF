// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHISwapChain.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHICommandQueue.h"
#include "RHI/Windows/WindowsRHIDevice.h"
#include "RHI/Windows/WindowsRHIFactory.h"
#include "PlatformMisc/IPlatformWindow.h"

WindowsRHISwapChain::WindowsRHISwapChain(std::shared_ptr<WindowsRHICommandQueue> queue, IPlatformWindow* drawingWindow)
	: RHISwapChain(queue->GetDevice())
{
	auto wDevice = std::static_pointer_cast<WindowsRHIDevice>(queue->GetDevice());
	auto wFactory = std::static_pointer_cast<WindowsRHIFactory>(wDevice->GetFactory());

	auto* dxgiFactory = wFactory->GetFactory();
	auto drawingSize = drawingWindow->GetDrawingSize();

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc =
	{
		.Width = (UINT)drawingSize.X,
		.Height = (UINT)drawingSize.Y,
		.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
		.SampleDesc = { 1, 0 },
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = 3,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags = 0
	};

	ComPtr<IDXGISwapChain1> swapChain1;
	HR(dxgiFactory->CreateSwapChainForHwnd(queue->GetQueue(), (HWND)drawingWindow->GetPointer(), &swapChainDesc, nullptr, nullptr, &swapChain1));
	HR(swapChain1.As(&_swapChain));
}

WindowsRHISwapChain::~WindowsRHISwapChain() noexcept
{
}

void WindowsRHISwapChain::Present()
{
	HR(_swapChain->Present(0, 0));
}

#endif