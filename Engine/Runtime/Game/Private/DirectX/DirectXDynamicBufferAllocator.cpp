// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXDynamicBufferAllocator.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"
#include "Logging/LogMacros.h"

using namespace std;

DirectXDynamicBufferAllocator::SingleBufferAllocator::SingleBufferAllocator(DirectXDynamicBufferAllocator* inSuper, size_t myCapacity) : Super()
	, super(inSuper)
	, myCapacity(myCapacity)
{
	// Ready buffer.
	heap = inSuper->GetDevice()->CreateDynamicBuffer(inSuper->AllocateUnit * myCapacity);
	HR(heap->Map(0, nullptr, (void**)&bufferPtr));

	// Ready index.
	for (size_t i = 0; i < myCapacity; ++i)
	{
		freeIndex.push(i);
	}
}

DirectXDynamicBufferAllocator::SingleBufferAllocator::~SingleBufferAllocator()
{

}

auto DirectXDynamicBufferAllocator::SingleBufferAllocator::Alloc() -> SingleBufferAllocatorHandle
{
	if (freeIndex.empty())
	{
		// There is no capacity.
		return { };
	}

	// Issue number.
	size_t front = freeIndex.front();
	freeIndex.pop();

	// Calc buffer start location.
	uint64 step = super->AllocateUnit * front;

	SingleBufferAllocatorHandle handle;
	handle.Allocator = this;
	handle.Resource = heap.Get();
	handle.GPUVirtualAddress = heap->GetGPUVirtualAddress() + step;
	handle.BufferPointer = bufferPtr + step;
	handle.Index = front;

	return handle;
}

void DirectXDynamicBufferAllocator::SingleBufferAllocator::Free(SingleBufferAllocatorHandle&& handle)
{
	freeIndex.push(handle.Index);
	handle = { };
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DirectXDynamicBufferAllocator::DirectXDynamicBufferAllocator(DirectXDeviceBundle* deviceBundle, uint64 allocateUnit, bool bThreadSafe) : Super()
	, allocateUnit(allocateUnit)
	, bThreadSafe(bThreadSafe)
{
	SetDeviceChildPtr(nullptr, deviceBundle);
}

DirectXDynamicBufferAllocator::~DirectXDynamicBufferAllocator()
{

}

void DirectXDynamicBufferAllocator::ShrinkToFit()
{
	{
		unique_lock<mutex> lock;

		if (bThreadSafe)
		{
			// Move lock.
			lock = unique_lock<mutex>(locker);
		}

		auto it = allocators.rbegin();
		for (; it != allocators.rend(); ++it)
		{
			// Find last empty block.
			if (!(*it)->Empty)
			{
				break;
			}
		}

		// Erase empty allocators.
		allocators.erase(it.base(), allocators.rbegin().base());
	}  // ~unique_lock
}

auto DirectXDynamicBufferAllocator::Alloc() -> SingleBufferAllocatorHandle
{
	{
		unique_lock<mutex> lock;

		if (bThreadSafe)
		{
			// Move lock.
			lock = unique_lock<mutex>(locker);
		}

		// Find allocator that have capacity.
		SingleBufferAllocator* allocator = nullptr;
		for (size_t i = 0; i < allocators.size(); ++i)
		{
			if (allocators[i]->Slack != 0)
			{
				allocator = allocators[i].Get();
				break;
			}
		}

		if (allocator == nullptr)
		{
			// Make new allocator.
			allocator = allocators.emplace_back(NewObject<SingleBufferAllocator>(this, 64)).Get();
		}

		return allocator->Alloc();
	}  // ~unique_lock
}

void DirectXDynamicBufferAllocator::Free(SingleBufferAllocatorHandle&& handle)
{
	if (!handle.IsValid)
	{
		SE_LOG(LogDirectX, Warning, L"Buffer handle is invalid. (!handle.IsValid)");
		return;
	}

	{
		unique_lock<mutex> lock;

		if (bThreadSafe)
		{
			// Move lock.
			lock = unique_lock<mutex>(locker);
		}

		handle.Allocator->Free(move(handle));
	}  // ~unique_lock
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DirectXDynamicBuffer::DirectXDynamicBuffer(DirectXDynamicBufferAllocator* inAllocator) : Super()
	, allocator(inAllocator)
	, handle(allocator->Alloc())
{

}

DirectXDynamicBuffer::~DirectXDynamicBuffer()
{
	Dispose(false);
}

void DirectXDynamicBuffer::Dispose()
{
	Dispose(true);
}

DirectXDynamicBufferAllocator* DirectXDynamicBuffer::GetAllocator() const
{
	return allocator.Get();
}

uint64 DirectXDynamicBuffer::GetGPUVirtualAddress() const
{
	return handle.GPUVirtualAddress;
}

int8* DirectXDynamicBuffer::GetBufferPointer() const
{
	return handle.BufferPointer;
}

void DirectXDynamicBuffer::Dispose(bool bDisposing)
{
	allocator->Free(move(handle));
}