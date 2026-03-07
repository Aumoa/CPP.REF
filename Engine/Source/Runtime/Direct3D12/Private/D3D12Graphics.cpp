// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "D3D12Graphics.h"
#include "GenericWindow.h"
#include "DXGISwapchainExt.h"
#include "D3D12CommandBuffer.h"
#include "D3D12Buffer.h"
#include "CommandQueue.h"
#include "D3D12RaytracingRenderPipeline.h"
#include "D3D12Shader.h"

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
		DXSetName(m_Device);
		
		m_CommandQueue[0] = std::make_unique<CommandQueue>(m_Device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
		m_CommandQueue[1] = std::make_unique<CommandQueue>(m_Device.Get(), D3D12_COMMAND_LIST_TYPE_COMPUTE);
		m_CommandQueue[2] = std::make_unique<CommandQueue>(m_Device.Get(), D3D12_COMMAND_LIST_TYPE_COPY);
	}

	D3D12Graphics::~D3D12Graphics() noexcept
	{
	}

	void D3D12Graphics::Dispose() noexcept
	{
		m_DXGI.Reset();
		m_Device.Reset();
		m_CommandQueue = {};
	}

	SharedPtr<GenericWindowSwapchainExtension> D3D12Graphics::InstallSwapChain(SharedPtr<GenericWindow> targetWindow)
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
		HR(m_DXGI->CreateSwapChain(m_CommandQueue[0]->GetQueue(), &swapchainDesc, &swapchain));

		ComPtr<IDXGISwapChain3> swapchain3;
		HR(swapchain.As(&swapchain3));

		auto extension = New<DXGISwapchainExt>(this, std::move(swapchain3));
		targetWindow->AddExtension(extension);
		return extension;
	}

	SharedPtr<CommandBuffer> D3D12Graphics::CreateCommandBuffer()
	{
		return New<D3D12CommandBuffer>(this);
	}

	SharedPtr<RenderPipeline> D3D12Graphics::CreateRaytracingRenderPipeline(std::vector<SharedPtr<Shader>> shaders)
	{
		return New<D3D12RaytracingRenderPipeline>(m_Device.Get(), std::move(shaders));
	}

	SharedPtr<Shader> D3D12Graphics::CreateShader(ShaderCreationInfo shaderCreationInfo)
	{
		return New<D3D12Shader>(std::move(shaderCreationInfo));
	}

	void D3D12Graphics::BeginRenderFrame()
	{
		static constexpr TimeSpan _1s = TimeSpan::FromSeconds(1);
		m_CommandQueue[0]->WaitForCompletion(m_LastFrameFenceValue, _1s);
	}

	void D3D12Graphics::EndRenderFrame()
	{
		++m_FrameCount;
		m_LastFrameFenceValue = m_CommandQueue[0]->Signal();
	}

	void D3D12Graphics::WaitForCompletion()
	{
		m_CommandQueue[0]->WaitForCompletion();
		m_CommandQueue[1]->WaitForCompletion();
		m_CommandQueue[2]->WaitForCompletion();
	}
}