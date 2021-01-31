// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "D3D12DescriptorIndex.h"

class D3D12DescriptorAllocator;

struct D3D12OfflineDescriptorIndex : public D3D12DescriptorIndex
{
	using Super = D3D12DescriptorIndex;
	using This = D3D12OfflineDescriptorIndex;

	D3D12OfflineDescriptorIndex();
	D3D12OfflineDescriptorIndex(size_t poolIndex, const std::vector<size_t>& rev_pool_ref, size_t revision, size_t index, D3D12_CPU_DESCRIPTOR_HANDLE handle);
	D3D12OfflineDescriptorIndex(size_t poolIndex, const Super& super);
	~D3D12OfflineDescriptorIndex();

	const size_t PoolIndex;
};