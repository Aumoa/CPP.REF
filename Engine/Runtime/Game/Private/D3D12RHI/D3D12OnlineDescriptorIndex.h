// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "D3D12DescriptorIndex.h"

#include "D3D12Minimal.h"

class D3D12DescriptorAllocator;
class D3D12OnlineDescriptorManager;

struct D3D12OnlineDescriptorIndex : public D3D12DescriptorIndex
{
	using Super = D3D12DescriptorIndex;
	using This = D3D12OnlineDescriptorIndex;

	D3D12OnlineDescriptorIndex();
	D3D12OnlineDescriptorIndex(ID3D12DescriptorHeap* inTable, D3D12OnlineDescriptorManager* inManager, const std::vector<size_t>& rev_pool_ref, size_t revision, size_t index, D3D12_CPU_DESCRIPTOR_HANDLE handle);
	D3D12OnlineDescriptorIndex(ID3D12DescriptorHeap* inTable, D3D12OnlineDescriptorManager* inManager, const Super& super);
	~D3D12OnlineDescriptorIndex();

	void Free();

	ID3D12DescriptorHeap* const pTable;
	D3D12OnlineDescriptorManager* const pManager;
};