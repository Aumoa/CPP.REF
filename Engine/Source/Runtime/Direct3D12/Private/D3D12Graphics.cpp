// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12Graphics.h"
#include "GenericWindow.h"
#include "DXGISwapchainExt.h"
#include "D3D12CommandBuffer.h"

namespace Ayla
{
	D3D12Graphics::D3D12Graphics()
	{
		UINT dxgiFactoryFlags = 0;
#if DO_CHECK
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

		// Enable the D3D12 debug layer if available. This must be called
		// before D3D12CreateDevice.
		{
			Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
			if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
			{
				debugController->EnableDebugLayer();
			}
		}
#endif

		HR(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_DXGI)));
		HR(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_Device)));
		
#if DO_CHECK
		HR(m_Device->SetName(L"D3D12Graphics.m_Device"));
#endif
		
		D3D12_COMMAND_QUEUE_DESC queueDesc =
		{
			.Type = D3D12_COMMAND_LIST_TYPE_DIRECT,
			.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
			.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE,
			.NodeMask = 0
		};
		HR(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)));
#if DO_CHECK
		HR(m_CommandQueue->SetName(L"D3D12Graphics.m_CommandQueue"));
#endif

		HR(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)));
#if DO_CHECK
		HR(m_Fence->SetName(L"D3D12Graphics.m_Fence"));
#endif
		m_FenceEvent = CreateEventExW(NULL, NULL, 0, GENERIC_ALL);
	}

	D3D12Graphics::~D3D12Graphics() noexcept
	{
		if (m_FenceEvent)
		{
			CloseHandle(m_FenceEvent);
			m_FenceEvent = NULL;
		}
	}

	void D3D12Graphics::Dispose() noexcept
	{
		m_DXGI.Reset();
		m_Device.Reset();
		m_CommandQueue.Reset();
		m_Fence.Reset();

		if (m_FenceEvent)
		{
			CloseHandle(m_FenceEvent);
			m_FenceEvent = NULL;
		}
	}

	SharedPtr<GenericWindowSwapchainExtension> D3D12Graphics::InstallSwapChain_Implementation(SharedPtr<GenericWindow> targetWindow)
	{
		auto windowSize = targetWindow->GetSize();

		DXGI_SWAP_CHAIN_DESC swapchainDesc =
		{
			.BufferDesc =
			{
				.Width = (UINT)windowSize.X,
				.Height = (UINT)windowSize.Y,
				.RefreshRate =
				{
					.Numerator = 0,
					.Denominator = 1
				},
				.Format = DXGI_FORMAT_B8G8R8A8_UNORM,
				.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
				.Scaling = DXGI_MODE_SCALING_UNSPECIFIED
			},
			.SampleDesc =
			{
				.Count = 1,
				.Quality = 0
			},
			.BufferUsage = DXGI_USAGE_BACK_BUFFER | DXGI_USAGE_RENDER_TARGET_OUTPUT,
			.BufferCount = (UINT)Graphics::kMaxSwapchainImages,
			.OutputWindow = (HWND)targetWindow->GetOSWindowHandle(),
			.Windowed = TRUE,
			.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
			.Flags = 0
		};

		ComPtr<IDXGISwapChain> swapchain;
		HR(m_DXGI->CreateSwapChain(m_CommandQueue.Get(), &swapchainDesc, &swapchain));

		ComPtr<IDXGISwapChain3> swapchain3;
		HR(swapchain.As(&swapchain3));

		auto extension = New<DXGISwapchainExt>(this, std::move(swapchain3));
		targetWindow->AddExtension(extension);
		return extension;
	}

	SharedPtr<CommandBuffer> D3D12Graphics::CreateCommandBuffer_Implementation()
	{
		return New<D3D12CommandBuffer>(this);
	}

	void D3D12Graphics::BeginRenderFrame()
	{
		static constexpr TimeSpan _1s = TimeSpan::FromSeconds(1);

		if (m_FenceValue > m_Fence->GetCompletedValue())
		{
			HR(m_Fence->SetEventOnCompletion((UINT64)m_FenceValue, m_FenceEvent));
			auto w = WaitForSingleObject(m_FenceEvent, (DWORD)_1s.GetTotalMilliseconds());
			check(w == WAIT_OBJECT_0);
		}
	}

	void D3D12Graphics::EndRenderFrame()
	{
		++m_FrameCount;
		HR(m_CommandQueue->Signal(m_Fence.Get(), ++m_FenceValue));
	}

	void D3D12Graphics::WaitForCompletion()
	{
		if (m_FenceValue > m_Fence->GetCompletedValue())
		{
			HR(m_Fence->SetEventOnCompletion((UINT64)m_FenceValue, m_FenceEvent));
			auto w = WaitForSingleObject(m_FenceEvent, INFINITE);
			check(w == WAIT_OBJECT_0);
		}
	}
}