// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DirectXDeviceChild.h"

namespace libty::inline DirectX
{
	class SDirectXRenderPass;

	class DIRECTX_API SDirectXCommandList : public SDirectXDeviceChild, implements(IRHIGraphicsCommandList)
	{
		GENERATED_BODY(SDirectXCommandList);

	public:
		ComPtr<ID3D12GraphicsCommandList4> pCommandList;

	public:
		SDirectXCommandList(SDirectXDevice* Owner, ComPtr<ID3D12CommandAllocator> pAllocator, ComPtr<ID3D12GraphicsCommandList4> pCommandList);

		using Super::Dispose;

		virtual void Reset(IRHICommandAllocator* pAllocator, IRHIPipelineState* pInitialState) override;
		virtual void Close() override;
		virtual void ResourceBarrier(std::span<const RHIResourceBarrier> barriers) override;
		virtual void ClearRenderTargetView(IRHIRenderTargetView* pRTV, size_t indexOf, const Color& color, std::span<const RectN> rects) override;

	protected:
		virtual void Dispose(bool bDisposing) override;
	};
}