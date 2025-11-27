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
		m_SwapchainRenderTexture = New<DXGISwapchainRenderTexture>(m_Swapchain);
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
		if (m_PendingFrameNumber == -1 || m_PendingFrameNumber > (int64)m_Owner->GetFrameNumber() || !m_PendingResize.has_value())
		{
			return;
		}

		m_PendingFrameNumber = -1;

		auto newSize = m_PendingResize.value();
		m_PendingResize.reset();

		m_SwapchainRenderTexture->ReleaseResources();
		HR(m_Swapchain->ResizeBuffers(0, (UINT)newSize.X, (UINT)newSize.Y, DXGI_FORMAT_B8G8R8A8_UNORM, 0));
		m_SwapchainRenderTexture->AllocateResources();
	}

	void DXGISwapchainExt::OnResize(const Vector2N& newSize)
	{
		m_PendingResize = newSize;
		m_PendingFrameNumber = m_Owner->GetFrameNumber() + 1;
	}
}