// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12OfflineDescriptorManager.h"

#include "D3D12OfflineDescriptorIndex.h"
#include "D3D12DescriptorAllocator.h"
#include <optional>
#include "Logging/LogMacros.h"

using namespace std;

D3D12OfflineDescriptorManager::D3D12OfflineDescriptorManager(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t numDescriptorsPerAllocator) : Super()
	, Type(type)
	, NumDescriptorsPerAllocator(numDescriptorsPerAllocator)
	, device(nullptr)
{
	using T = ID3D12Device*;
	this->device = device;
	heapAllocators.emplace_back(NewAllocator());
}

D3D12OfflineDescriptorManager::~D3D12OfflineDescriptorManager()
{

}

D3D12OfflineDescriptorIndex D3D12OfflineDescriptorManager::Alloc()
{
	ScopedLock(locker);

	optional<size_t> select;
	for (size_t i = 0; i < heapAllocators.size(); ++i)
	{
		if (heapAllocators[i]->Slack != 0)
		{
			select = i;
			break;
		}
	}

	if (!select.has_value())
	{
		select = heapAllocators.size();
		heapAllocators.emplace_back(NewAllocator());
	}

	D3D12DescriptorAllocator* allocator = heapAllocators[*select].Get();
	D3D12DescriptorIndex index = allocator->Alloc();
	auto offline_index = D3D12OfflineDescriptorIndex(*select, index);

	return offline_index;
}

void D3D12OfflineDescriptorManager::Free(const D3D12OfflineDescriptorIndex& index)
{
	ScopedLock(locker);

	if (!index.IsValid)
	{
		return;
	}

	if (index.PoolIndex >= heapAllocators.size())
	{
		SE_LOG(LogD3D12RHI, Error, L"Heap pool index{0} is not valid.", index.PoolIndex);
		return;
	}

	D3D12DescriptorAllocator* allocator = heapAllocators[index.PoolIndex].Get();
	allocator->Free(index);
}

TRefPtr<D3D12DescriptorAllocator> D3D12OfflineDescriptorManager::NewAllocator()
{
	return NewObject<D3D12DescriptorAllocator>(device, Type, NumDescriptorsPerAllocator, true, false);
}