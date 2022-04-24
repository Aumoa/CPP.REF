// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DirectXDeviceChild.h"

namespace libty::inline DirectX
{
	class DIRECTX_API SDirectXCommandQueue : public SDirectXDeviceChild, implements(IRHICommandQueue)
	{
		GENERATED_BODY(SDirectXCommandQueue);

	public:
		ComPtr<ID3D12CommandQueue> pQueue;

	public:
		SDirectXCommandQueue(IRHIDevice* Owner, ComPtr<ID3D12CommandQueue> pQueue);

		using Super::Dispose;

		virtual void ExecuteCommandBuffers(std::span<IRHIGraphicsCommandList* const> commandLists) override;
		virtual void Signal(IRHIFence* pFence, uint64 fenceValue) override;

	protected:
		virtual void Dispose(bool bDisposing) override;
	};
}