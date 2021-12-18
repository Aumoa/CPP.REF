// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "DirectXCommon.h"
#include "RHI/IRHIView.h"

class SD3D12View : public SD3D12DeviceChild, implements IRHIView
{
	GENERATED_BODY(SD3D12View)

private:
	ComPtr<ID3D12DescriptorHeap> Heap;
	SPROPERTY(Resources)
	std::vector<IRHIResource*> Resources;

	D3D12_CPU_DESCRIPTOR_HANDLE Base;
	uint32 IncrementSize = 0;

public:
	SD3D12View(SDXGIFactory* InFactory, SD3D12Device* InDevice, ComPtr<ID3D12DescriptorHeap> Heap, size_t NumResources, D3D12_DESCRIPTOR_HEAP_TYPE HeapType);

	using Super::Dispose;
	virtual int32 GetViewCount() override;
	virtual IRHIResource* GetResource(int32 IndexOf) override;

	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(int32 IndexOf);
	void AssignResource(int32 IndexOf, IRHIResource* InResource);

protected:
	virtual void Dispose(bool bDispsoing) override;
};