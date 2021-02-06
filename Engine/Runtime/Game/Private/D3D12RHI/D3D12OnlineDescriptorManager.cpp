// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12OnlineDescriptorManager.h"

#include "D3D12OnlineDescriptorIndex.h"
#include "D3D12DescriptorAllocator.h"
#include "Logging/LogMacros.h"

using namespace std;

D3D12OnlineDescriptorManager::D3D12OnlineDescriptorManager(ID3D12Device* device, D3D12_DESCRIPTOR_HEAP_TYPE type, size_t inNumDescriptors) : Super()
	, Type(type)
	, NumDescriptors(inNumDescriptors)
	, device(nullptr)
{
	this->device = device;
	singleHeapAllocator = NewObject<D3D12DescriptorAllocator>(device, type, inNumDescriptors, false);
}

D3D12OnlineDescriptorManager::~D3D12OnlineDescriptorManager()
{

}

D3D12OnlineDescriptorIndex D3D12OnlineDescriptorManager::Alloc()
{
	D3D12DescriptorAllocator* allocator = singleHeapAllocator.Get();
	D3D12DescriptorIndex index = allocator->Alloc();
	return D3D12OnlineDescriptorIndex(allocator->pHeap, this, index);
}

void D3D12OnlineDescriptorManager::Free(const D3D12OnlineDescriptorIndex& index)
{
	if (!index.IsValid)
	{
		return;
	}

	D3D12DescriptorAllocator* allocator = singleHeapAllocator.Get();
	allocator->Free(index);
}

ID3D12DescriptorHeap* D3D12OnlineDescriptorManager::pHeap_get() const
{
	return singleHeapAllocator->pHeap;
}