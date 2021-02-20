// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"
#include <mutex>

class D3D12DescriptorAllocator;
struct D3D12OfflineDescriptorIndex;

class D3D12OfflineDescriptorManager : public Object
{
public:
	using Super = Object;
	using This = D3D12OfflineDescriptorManager;

private:
	const D3D12_DESCRIPTOR_HEAP_TYPE Type;
	const size_t NumDescriptorsPerAllocator;

	ID3D12Device* device;
	std::vector<TRefPtr<D3D12DescriptorAllocator>> heapAllocators;
	std::mutex locker;

public:
	D3D12OfflineDescriptorManager(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t numDescriptorsPerAllocator);
	~D3D12OfflineDescriptorManager() override;

	D3D12OfflineDescriptorIndex Alloc();
	void Free(const D3D12OfflineDescriptorIndex& index);

private:
	TRefPtr<D3D12DescriptorAllocator> NewAllocator();
};