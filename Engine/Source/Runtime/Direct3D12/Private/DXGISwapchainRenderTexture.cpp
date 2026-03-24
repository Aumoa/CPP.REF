// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "DXGISwapchainRenderTexture.h"

namespace Ayla
{
	DXGISwapchainRenderTexture::DXGISwapchainRenderTexture(ID3D12Device* device, ComPtr<IDXGISwapChain3> swapchain)
		: m_Device(device)
		, m_Swapchain(std::move(swapchain))
	{
		HR(m_Swapchain->GetDesc(&m_SwapchainDesc));

		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc =
		{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
			.NumDescriptors = (UINT)m_SwapchainDesc.BufferCount,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			.NodeMask = 0
		};
		HR(m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RTVHeap)));
		DXSetName(m_RTVHeap);

		D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc =
		{
			.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
			.NumDescriptors = 1,
			.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE,
			.NodeMask = 0
		};
		HR(m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DSVHeap)));
		DXSetName(m_DSVHeap);

		AllocateResources();
	}

	DXGISwapchainRenderTexture::~DXGISwapchainRenderTexture() noexcept
	{
	}

	Vector2N DXGISwapchainRenderTexture::GetSize() const
	{
		auto& bd = m_SwapchainDesc.BufferDesc;
		return Vector2N((int32)bd.Width, (int32)bd.Height);
	}

	void DXGISwapchainRenderTexture::Acquire(CommandBuffer* cmd)
	{
		m_CurrentBackBufferIndex = (uint32)m_Swapchain->GetCurrentBackBufferIndex();
	}

	void DXGISwapchainRenderTexture::ReleaseResources()
	{
		m_SwapchainResources.clear();
	}

	void DXGISwapchainRenderTexture::AllocateResources()
	{
		m_SwapchainResources.resize((size_t)m_SwapchainDesc.BufferCount);

		auto rtvHandle = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
		for (size_t i = 0; i < (size_t)m_SwapchainDesc.BufferCount; ++i)
		{
			HR(m_Swapchain->GetBuffer((UINT)i, IID_PPV_ARGS(&m_SwapchainResources[i])));
			DXSetName(m_SwapchainResources[i]);

			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = m_SwapchainDesc.BufferDesc.Format;
			rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			m_Device->CreateRenderTargetView(m_SwapchainResources[i].Get(), &rtvDesc, rtvHandle);
			rtvHandle.ptr += m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		}

		D3D12_HEAP_PROPERTIES heapProp =
		{
			.Type = D3D12_HEAP_TYPE_DEFAULT,
			.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,
			.CreationNodeMask = 1,
			.VisibleNodeMask = 1
		};

		D3D12_RESOURCE_DESC depthStencilDesc =
		{
			.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D,
			.Alignment = 0,
			.Width = m_SwapchainDesc.BufferDesc.Width,
			.Height = m_SwapchainDesc.BufferDesc.Height,
			.DepthOrArraySize = 1,
			.MipLevels = 1,
			.Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
			.SampleDesc =
			{
				.Count = 1,
				.Quality = 0
			},
			.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
			.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		};

		D3D12_CLEAR_VALUE clearValue = {};
		clearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		clearValue.DepthStencil.Depth = 1.0f;
		clearValue.DepthStencil.Stencil = 0;

		HR(m_Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &depthStencilDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &clearValue, IID_PPV_ARGS(&m_DepthStencilResource)));
		DXSetName(m_DepthStencilResource);

		D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		m_Device->CreateDepthStencilView(m_DepthStencilResource.Get(), &dsvDesc, m_DSVHeap->GetCPUDescriptorHandleForHeapStart());
	}

	ID3D12Resource* DXGISwapchainRenderTexture::GetCurrentBackBuffer() const
	{
		return m_SwapchainResources[m_CurrentBackBufferIndex].Get();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DXGISwapchainRenderTexture::GetRTVDescriptorHandle() const
	{
		auto rtvHandle = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += (SIZE_T)m_CurrentBackBufferIndex * m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		return rtvHandle;
	}

	D3D12_CPU_DESCRIPTOR_HANDLE DXGISwapchainRenderTexture::GetDSVDescriptorHandle() const
	{
		return m_DSVHeap->GetCPUDescriptorHandleForHeapStart();
	}
}