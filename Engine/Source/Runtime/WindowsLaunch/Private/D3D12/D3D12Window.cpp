// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12/D3D12Window.h"
#include "D3D12/D3D12Graphics.h"
#include "GenericPlatform/GenericWindow.h"

namespace Ayla
{
	void D3D12Window::Initialize(RPtr<D3D12Graphics> graphics, RPtr<GenericWindow> targetWindow)
	{
		auto targetWindowSize = targetWindow->GetSize();

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc =
		{
			.Width = (UINT)targetWindowSize.X,
			.Height = (UINT)targetWindowSize.Y,
			.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
			.Stereo = FALSE,
			.SampleDesc =
			{
				.Count = 1,
				.Quality = 0
			},
			.BufferUsage = DXGI_USAGE_BACK_BUFFER,
			.BufferCount = 2,
			.Scaling = DXGI_SCALING_STRETCH,
			.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
			.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
			.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING
		};
		HR(graphics->m_DXGI->CreateSwapChainForHwnd(graphics->m_PrimaryCommandQueue.Get(), (HWND)targetWindow->GetOSWindowHandle(), &swapChainDesc, NULL, NULL, &m_SwapChain));
		HR(graphics->m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
		m_FenceEvent = CreateEventExW(NULL, NULL, 0, GENERIC_ALL);
	}

	void D3D12Window::Present()
	{
		if (m_FenceValue != 0)
		{
			if (m_Fence->GetCompletedValue() < m_FenceValue)
			{
				HR(m_Fence->SetEventOnCompletion(m_FenceValue, m_FenceEvent));
				WaitForSingleObject(m_FenceEvent, INFINITE);
			}
		}

		HR(m_SwapChain->Present(0, 0));
		HR(m_Fence->Signal(++m_FenceValue));
	}
}