// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"

class D3D12DescriptorAllocator;
struct D3D12OnlineDescriptorIndex;

class D3D12OnlineDescriptorManager : public Object
{
public:
	using Super = Object;
	using This = D3D12OnlineDescriptorManager;

private:
	const D3D12_DESCRIPTOR_HEAP_TYPE Type;
	const size_t NumDescriptors;

	ID3D12Device* device;
	TRefPtr<D3D12DescriptorAllocator> singleHeapAllocator;

public:
	D3D12OnlineDescriptorManager(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t inNumDescriptors);
	~D3D12OnlineDescriptorManager() override;

	D3D12OnlineDescriptorIndex Alloc();
	void Free(const D3D12OnlineDescriptorIndex& index);

	vs_property_get(ID3D12DescriptorHeap*, pHeap);
	ID3D12DescriptorHeap* pHeap_get() const;
};