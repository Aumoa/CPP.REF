// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DirectXView.h"

namespace libty::inline DirectX
{
	class DIRECTX_API SDirectXShaderResourceView : public SDirectXView, implements(IRHIShaderResourceView)
	{
		GENERATED_BODY(SDirectXShaderResourceView);

	public:
		SDirectXShaderResourceView(SDirectXDevice* Owner, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap, size_t NumViews);

		virtual void CreateShaderResourceView(size_t index, IRHIResource* pResource, const RHIShaderResourceViewDesc* pDesc) override;
		virtual void CreateUnorderedAccessView(size_t index, IRHIResource* pResource, IRHIResource* pCounter, const RHIUnorderedAccessViewDesc* pDesc) override;
	};
}