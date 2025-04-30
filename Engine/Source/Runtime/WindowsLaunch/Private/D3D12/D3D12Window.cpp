// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12/D3D12Window.h"
#include "D3D12/D3D12Graphics.h"
#include "GenericPlatform/GenericWindow.h"
#include "D2D1/IMGUI/D2D1GUI.h"

namespace Ayla
{
	void D3D12Window::Initialize(RPtr<D3D12Graphics> graphics, RPtr<GenericWindow> targetWindow)
	{
		auto targetWindowSize = targetWindow->GetSize();
		m_CommandQueue = graphics->m_PrimaryCommandQueue;
		m_Device11On12 = graphics->m_Device11On12;
		m_DeviceContext11 = graphics->m_DeviceContext11;

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
			.Scaling = DXGI_SCALING_NONE,
			.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
			.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED,
			.Flags = 0
		};
		ComPtr<IDXGISwapChain1> swapChain;
		HR(graphics->m_DXGI->CreateSwapChainForHwnd(graphics->m_PrimaryCommandQueue.Get(), (HWND)targetWindow->GetOSWindowHandle(), &swapChainDesc, NULL, NULL, &swapChain));
		HR(swapChain.As(&m_SwapChain));
		HR(graphics->m_Device2D->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &m_GUIContext));

		D3D11_RESOURCE_FLAGS flags =
		{
			.BindFlags = D3D11_BIND_RENDER_TARGET
		};

		for (size_t i = 0; i < AE_ARRAYSIZE(m_BackBuffers); ++i)
		{
			HR(m_SwapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&m_BackBuffers[i])));
			HR(graphics->m_Device11On12->CreateWrappedResource(m_BackBuffers[i].Get(), &flags, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT, IID_PPV_ARGS(&m_BackBuffersInt[i])));
			HR(graphics->m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_Allocator[i])));
			ComPtr<IDXGISurface1> dxgiSurface;
			HR(m_BackBuffersInt[i].As(&dxgiSurface));
			auto bitmapPropes = D2D1::BitmapProperties1(
				D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
				D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				96.0f,
				96.0f,
				NULL
			);
			HR(m_GUIContext->CreateBitmapFromDxgiSurface(dxgiSurface.Get(), bitmapPropes, &m_Bitmaps[i]));
		}

		HR(graphics->m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_Allocator[0].Get(), NULL, IID_PPV_ARGS(&m_CommandList)));
		HR(m_CommandList->Close());

		m_TargetWindow = targetWindow;
		m_Graphics = graphics;
		m_GUI = New<D2D1GUI>();
	}

	Vector2F D3D12Window::GetSize()
	{
		return Vector<>::Cast<Vector2F>(m_TargetWindow->GetSize());
	}

	void D3D12Window::BeginGUI()
	{
		size_t index = (size_t)m_SwapChain->GetCurrentBackBufferIndex();
		HR(m_Allocator[index]->Reset());
		HR(m_CommandList->Reset(m_Allocator[index].Get(), NULL));

		D3D12_RESOURCE_BARRIER barrier =
		{
			.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION,
			.Transition =
			{
				.pResource = m_BackBuffers[index].Get(),
				.StateBefore = D3D12_RESOURCE_STATE_PRESENT,
				.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET
			}
		};

		m_CommandList->ResourceBarrier(1, &barrier);
		HR(m_CommandList->Close());

		m_CommandQueue->ExecuteCommandLists(1, (ID3D12CommandList**)m_CommandList.GetAddressOf());

		ID3D11Resource** targetResources = m_BackBuffersInt[index].GetAddressOf();
		m_Device11On12->AcquireWrappedResources(targetResources, 1);
		m_GUIContext->SetTarget(m_Bitmaps[index].Get());
		m_GUIContext->BeginDraw();
		m_GUIContext->Clear(D2D1::ColorF(0, 0, 0, 0));

		m_GUI->BeginRender();
	}

	void D3D12Window::EndGUI()
	{
		m_GUI->EndRender(m_GUIContext.Get(), m_Graphics);

		size_t index = (size_t)m_SwapChain->GetCurrentBackBufferIndex();
		ID3D11Resource** targetResources = m_BackBuffersInt[index].GetAddressOf();

		HR(m_GUIContext->EndDraw());
		m_GUIContext->SetTarget(nullptr);
		m_Device11On12->ReleaseWrappedResources(targetResources, 1);
		m_DeviceContext11->Flush();

		HR(m_SwapChain->Present(0, 0));
	}
}