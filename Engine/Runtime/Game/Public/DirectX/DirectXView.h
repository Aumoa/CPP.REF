// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

#include "DirectXMinimal.h"

class GAME_API DirectXView : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	TComPtr<ID3D12DescriptorHeap> descriptorHeap;

	uint32 numDescriptors;
	uint32 incrementSize;
	size_t handleCPU;

public:
	DirectXView(DirectXDeviceBundle* deviceBundle, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32 numDescriptors);
	~DirectXView() override;

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(size_t index) const;

	vs_property_get_auto(uint32, NumDescriptors, numDescriptors);
	vs_property_get_auto(ID3D12DescriptorHeap*, Item, descriptorHeap.Get());
};