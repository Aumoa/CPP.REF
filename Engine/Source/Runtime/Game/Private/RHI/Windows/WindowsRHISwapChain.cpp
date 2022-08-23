// Copyright 2020-2022 Aumoa.lib. All right reserved.

#include "RHI/Windows/WindowsRHISwapChain.h"

#if PLATFORM_WINDOWS

#include "RHI/Windows/WindowsRHICommandQueue.h"
#include "RHI/Windows/WindowsRHIDevice.h"
#include "RHI/Windows/WindowsRHIFactory.h"
#include "RHI/Windows/WindowsRHIResource.h"
#include "PlatformMisc/IPlatformWindow.h"

std::shared_ptr<RHISwapChain> WindowsRHIDevice::CreateSwapChain(std::shared_ptr<RHICommandQueue> queue, IPlatformWindow* drawingWindow)
{
	return std::shared_ptr<RHISwapChain>(new WindowsRHISwapChain(std::static_pointer_cast<WindowsRHICommandQueue>(queue), drawingWindow));
}

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
		.BufferUsage = DXGI_USAGE_BACK_BUFFER,
		.BufferCount = 3,
		.Scaling = DXGI_SCALING_STRETCH,
		.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
		.Flags = 0
	};

	ComPtr<IDXGISwapChain1> swapChain1;
	HR(dxgiFactory->CreateSwapChainForHwnd(queue->GetQueue(), (HWND)drawingWindow->GetPointer(), &swapChainDesc, nullptr, nullptr, &swapChain1));
	HR(swapChain1.As(&_swapChain));

	AssignBuffers();
}

WindowsRHISwapChain::~WindowsRHISwapChain() noexcept
{
}

void WindowsRHISwapChain::Present()
{
	HR(_swapChain->Present(0, 0));
}

void WindowsRHISwapChain::ResizeBuffers(const Vector2N& size)
{
	_buffers.clear();
	HR(_swapChain->ResizeBuffers(0, (UINT)size.X, (UINT)size.Y, DXGI_FORMAT_UNKNOWN, 0));
	AssignBuffers();
}

size_t WindowsRHISwapChain::GetCurrentBackBufferIndex() const
{
	return (size_t)_swapChain->GetCurrentBackBufferIndex();
}

std::shared_ptr<RHIResource> WindowsRHISwapChain::GetBuffer(size_t index)
{
	return _buffers[index];
}

void WindowsRHISwapChain::AssignBuffers()
{
	for (size_t i = 0; i < 3; ++i)
	{
		ComPtr<ID3D12Resource> pResource;
		HR(_swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&pResource)));
		auto& ptr = _buffers.emplace_back(new WindowsRHIResource(GetDevice()));
		ptr->_resource = std::move(pResource);
	}
}

#endif