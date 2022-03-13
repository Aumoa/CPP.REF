// Copyright 2020-2022 Aumoa.lib. All right reserved.

#pragma once

#include "Object.h"
#include "DirectXDeviceChild.h"

class DIRECTX_API SDirectXView : public SDirectXDeviceChild, implements IRHIView
{
	GENERATED_BODY(SDirectXView)

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

	virtual int32 GetViewCount() override;
	virtual IRHIResource* GetResource(int32 IndexOf) override;

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(int32 IndexOf);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(int32 IndexOf);

protected:
	virtual void Dispose(bool bDisposing) override;
};