// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "DXGISwapchainRenderTexture.h"

namespace Ayla
{
	DXGISwapchainRenderTexture::DXGISwapchainRenderTexture(ComPtr<IDXGISwapChain3> swapchain)
		: m_Swapchain(std::move(swapchain))
	{
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
		HR(m_Swapchain->GetDesc(&m_SwapchainDesc));
		m_SwapchainResources.resize((size_t)m_SwapchainDesc.BufferCount);

		for (size_t i = 0; i < (size_t)m_SwapchainDesc.BufferCount; ++i)
		{
			HR(m_Swapchain->GetBuffer((UINT)i, IID_PPV_ARGS(&m_SwapchainResources[i])));
		}
	}
}