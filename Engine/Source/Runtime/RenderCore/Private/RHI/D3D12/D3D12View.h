// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "CoreMinimal.h"
#include "D3D12DeviceChild.h"
#include "RHI/IRHIView.h"
#include "RHI/DirectXCommon.h"

class SD3D12View : public SD3D12DeviceChild, implements IRHIView
{
	GENERATED_BODY(SD3D12View)

private:
	ComPtr<ID3D12DescriptorHeap> _heap;
	std::vector<std::shared_ptr<IRHIResource>> _resources;

	D3D12_CPU_DESCRIPTOR_HANDLE _base;
	uint32 _incrementSize = 0;

public:
	SD3D12View(SDXGIFactory* factory, SD3D12Device* device, ComPtr<ID3D12DescriptorHeap> heap, size_t resources, D3D12_DESCRIPTOR_HEAP_TYPE type);

	virtual IRHIResource* GetResource(int32 indexOf) override;

	D3D12_CPU_DESCRIPTOR_HANDLE GetHandle(int32 indexOf);
};