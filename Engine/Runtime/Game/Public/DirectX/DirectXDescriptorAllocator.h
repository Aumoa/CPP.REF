// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

class DirectXShaderResourceView;

class GAME_API DirectXDescriptorAllocator : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	ID3D12Device5* device;
	uint32 numDescriptors;
	uint32 issued;

	TComPtr<ID3D12DescriptorHeap> descriptorHeap;
	size_t handleStart;
	uint64 handleStartGpu;
	uint32 incrementSize;

public:
	DirectXDescriptorAllocator(DirectXDeviceBundle* deviceBundle);
	~DirectXDescriptorAllocator() override;

	void BeginAllocate(uint32 capacity);
	void EndAllocate();

	uint32 Issue(DirectXShaderResourceView* inSRV);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32 issueNumber) const;

	vs_property(uint32, Capacity);
};