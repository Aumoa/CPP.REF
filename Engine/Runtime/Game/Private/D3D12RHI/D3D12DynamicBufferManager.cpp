// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12DynamicBufferManager.h"

#include "D3D12DynamicBufferAllocator.h"
#include "D3D12DynamicBuffer.h"

D3D12DynamicBufferManager::D3D12DynamicBufferManager(size_t inNodeSize) : Super()
	, nodeSize(inNodeSize)
	, capacity(1024)
{

}

D3D12DynamicBufferManager::~D3D12DynamicBufferManager()
{

}

TRefPtr<D3D12DynamicBuffer> D3D12DynamicBufferManager::AllocBuffer()
{
	ScopedLock(locker);

	D3D12DynamicBufferAllocator* myAllocator = nullptr;
	for (size_t i = 0; i < allocators.size(); ++i)
	{
		if (allocators[i]->Slack != 0)
		{
			myAllocator = allocators[i].Get();
		}
	}

	if (myAllocator == nullptr)
	{
		myAllocator = allocators.emplace_back(NewObject<D3D12DynamicBufferAllocator>(capacity, nodeSize)).Get();
	}

	size_t alloc_idx = myAllocator->Alloc();
	return NewObject<D3D12DynamicBuffer>(alloc_idx, myAllocator);
}