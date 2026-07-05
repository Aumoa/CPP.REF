// Copyright 2020-2025 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12Common.h"
#include "GenericWindowSwapchainExtension.h"
#include "IGenericWindowResizeEventHandler.h"
#include "DXGISwapchainExt.gen.h"

namespace Ayla
{
	class D3D12Graphics;
	class DXGISwapchainRenderTexture;

	ACLASS()
	class DXGISwapchainExt : public GenericWindowSwapchainExtension, public IGenericWindowResizeEventHandler
	{
		GENERATED_BODY()

	private:
		D3D12Graphics* const m_Owner;
		ComPtr<IDXGISwapChain3> m_Swapchain;
		SharedPtr<DXGISwapchainRenderTexture> m_SwapchainRenderTexture;

		int64 m_PendingFrameNumber = -1;
		std::optional<Vector2N> m_PendingResize;

	public:
		DXGISwapchainExt(D3D12Graphics* owner, ComPtr<IDXGISwapChain3> swapchain);
		virtual ~DXGISwapchainExt() noexcept override;

		virtual PresentableRenderTarget* GetPresentableRenderTarget() override;
		virtual void Destroy() override;
		virtual void DoResize() override;

		virtual void OnResize(const Vector2N& newSize) override;
	};
}
