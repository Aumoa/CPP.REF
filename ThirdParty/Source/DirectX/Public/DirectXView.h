// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "DirectXDeviceChild.h"

namespace libty::inline DirectX
{
	class DIRECTX_API SDirectXView : public SDirectXDeviceChild, implements(IRHIView)
	{
		GENERATED_BODY(SDirectXView);

	public:
		ComPtr<ID3D12DescriptorHeap> pDescriptorHeap;
		SPROPERTY(BindResources)
		std::vector<IRHIResource*> BindResources;

	private:
		std::optional<D3D12_CPU_DESCRIPTOR_HANDLE> BaseCPUHandle;
		std::optional<D3D12_GPU_DESCRIPTOR_HANDLE> BaseGPUHandle;
		uint32 IncrementSize = 0;

	public:
		SDirectXView(SDirectXDevice* Owner, ComPtr<ID3D12DescriptorHeap> pDescriptorHeap, size_t NumViews);

		using Super::Dispose;

		virtual size_t GetViewCount() override;
		virtual IRHIResource* GetResource(size_t indexOf) override;

		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(size_t indexOf);
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(size_t indexOf);

	protected:
		virtual void Dispose(bool bDisposing) override;
	};
}