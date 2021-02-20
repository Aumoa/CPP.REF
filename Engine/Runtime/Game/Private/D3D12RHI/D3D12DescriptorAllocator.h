
// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"
#include <mutex>

struct D3D12DescriptorIndex;

class D3D12DescriptorAllocator : virtual public Object
{
public:
	using Super = Object;
	using This = D3D12DescriptorAllocator;

private:
	ID3D12Device* const DeviceRef;
	const uint32 IncrementSize;

	std::mutex locker;
	ComPtr<ID3D12DescriptorHeap> descriptorHeap;
	std::queue<size_t> descriptor_pool;
	std::vector<size_t> rev_pool;

	bool bOffline : 1;
	bool bThreadSafe : 1;

public:
	D3D12DescriptorAllocator(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t numDescriptors, bool bIsOffline, bool bThreadSafe = true);
	~D3D12DescriptorAllocator() override;

	D3D12DescriptorIndex Alloc();
	void Free(const D3D12DescriptorIndex& index);

	vs_property_get(size_t, Slack);
	size_t Slack_get() const;
	vs_property_get(bool, IsOffline);
	bool IsOffline_get() const;
	vs_property_get(bool, IsThreadSafe);
	bool IsThreadSafe_get() const;
	vs_property_get(ID3D12DescriptorHeap*, pHeap);
	ID3D12DescriptorHeap* pHeap_get() const;

	const D3D12_DESCRIPTOR_HEAP_TYPE Type;
	const size_t Count;

private:
	void InitializePool();
};