// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12Common.h"
#include "Rendering/RenderTexture.h"
#include "DXGISwapchainRenderTexture.gen.h"

namespace Ayla
{
	ACLASS()
	class DXGISwapchainRenderTexture : public RenderTexture
	{
		GENERATED_BODY()

	private:
		ComPtr<IDXGISwapChain3> m_Swapchain;
		DXGI_SWAP_CHAIN_DESC m_SwapchainDesc;
		std::vector<ComPtr<ID3D12Resource>> m_SwapchainResources;
		uint32 m_CurrentBackBufferIndex = 0;

	public:
		DXGISwapchainRenderTexture(ComPtr<IDXGISwapChain3> swapchain);
		virtual ~DXGISwapchainRenderTexture() noexcept override;

		virtual Vector2N GetSize() const override;

		virtual void Acquire(CommandBuffer* cmd) override;

		virtual void RenderRaytracing(CommandBuffer* cmd, const SceneView& view) override;

		void ReleaseResources();
		void AllocateResources();
	};
}