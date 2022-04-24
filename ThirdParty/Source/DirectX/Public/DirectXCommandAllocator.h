// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DirectXDeviceChild.h"

namespace libty::inline DirectX
{
	class DIRECTX_API SDirectXCommandAllocator : public SDirectXDeviceChild, implements(IRHICommandAllocator)
	{
		GENERATED_BODY(SDirectXCommandAllocator);

	public:
		ComPtr<ID3D12CommandAllocator> pAllocator;

	public:
		SDirectXCommandAllocator(SDirectXDevice* Owner, ComPtr<ID3D12CommandAllocator> pAllocator);

		using Super::Dispose;

		virtual void Reset() override;

	protected:
		virtual void Dispose(bool bDisposing) override;
	};
}