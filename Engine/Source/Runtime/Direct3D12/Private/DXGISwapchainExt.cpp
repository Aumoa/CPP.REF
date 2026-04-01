// Copyright 2020-2025 Aumoa.lib. All right reserved.

#include "DXGISwapchainExt.h"
#include "DXGISwapchainRenderTexture.h"
#include "D3D12Graphics.h"

namespace Ayla
{
	DXGISwapchainExt::DXGISwapchainExt(D3D12Graphics* owner, ComPtr<IDXGISwapChain3> swapchain)
		: m_Owner(owner)
		, m_Swapchain(std::move(swapchain))
	{
		m_SwapchainRenderTexture = New<DXGISwapchainRenderTexture>(m_Owner->GetDevice(), m_Swapchain);
	}

	DXGISwapchainExt::~DXGISwapchainExt() noexcept
	{
	}

	SharedPtr<RenderTexture> DXGISwapchainExt::GetRenderTexture()
	{
		return m_SwapchainRenderTexture;
	}

	void DXGISwapchainExt::Present(CommandBuffer* commandBuffer)
	{
		HR(m_Swapchain->Present(1, 0));
	}

	void DXGISwapchainExt::Destroy()
	{
		m_SwapchainRenderTexture->ReleaseResources();
		m_SwapchainRenderTexture.Release();
		m_Swapchain.Reset();
	}

	void DXGISwapchainExt::DoResize()
	{
		if (m_PendingFrameNumber == -1 || m_PendingFrameNumber > (int64)m_Owner->GetFrameIndex() || !m_PendingResize.has_value())
		{
			return;
		}

		m_PendingFrameNumber = -1;

		Vector2N newSize = m_PendingResize.value();
		m_PendingResize.reset();

		DXGI_SWAP_CHAIN_DESC desc;
		HR(m_Swapchain->GetDesc(&desc));

		m_SwapchainRenderTexture->ReleaseResources();
		HR(m_Swapchain->ResizeBuffers(0, (UINT)newSize.X, (UINT)newSize.Y, desc.BufferDesc.Format, 0));
		m_SwapchainRenderTexture->AllocateResources(true);
	}

	void DXGISwapchainExt::OnResize(const Vector2N& newSize)
	{
		m_PendingResize = newSize;
		m_PendingFrameNumber = m_Owner->GetFrameIndex() + 1;
	}
}