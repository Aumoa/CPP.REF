// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"
#include "IDisposable.h"

#include "DirectXMinimal.h"

class GAME_API DirectXDynamicBufferAllocator : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	class SingleBufferAllocator;

public:
	struct SingleBufferAllocatorHandle
	{
		SingleBufferAllocator* Allocator;
		ID3D12Resource* Resource;
		uint64 GPUVirtualAddress;
		int8* BufferPointer;
		size_t Index;

		vs_property_get_auto(bool, IsValid, Allocator != nullptr);
	};

private:
	class SingleBufferAllocator : virtual public Object
	{
	public:
		using Super = Object;

	private:
		DirectXDynamicBufferAllocator* super;
		TComPtr<ID3D12Resource> heap;
		size_t myCapacity;
		std::queue<size_t> freeIndex;
		int8* bufferPtr;

	public:
		SingleBufferAllocator(DirectXDynamicBufferAllocator* inSuper, size_t myCapacity);
		~SingleBufferAllocator() override;

		SingleBufferAllocatorHandle Alloc();
		void Free(SingleBufferAllocatorHandle&& handle);
		
		vs_property_get_auto(size_t, Slack, freeIndex.size());
		vs_property_get_auto(bool, Empty, Slack == myCapacity);
	};

private:
	const uint64 allocateUnit;
	const bool bThreadSafe : 1;

	std::mutex locker;
	std::vector<TRefPtr<SingleBufferAllocator>> allocators;

public:
	DirectXDynamicBufferAllocator(DirectXDeviceBundle* deviceBundle, uint64 allocateUnit, bool bThreadSafe);
	~DirectXDynamicBufferAllocator();

	void ShrinkToFit();
	SingleBufferAllocatorHandle Alloc();
	void Free(SingleBufferAllocatorHandle&& handle);

	vs_property_get_auto(bool, IsThreadSafe, bThreadSafe);
	vs_property_get_auto(uint64, AllocateUnit, allocateUnit);
};

class GAME_API DirectXDynamicBuffer : virtual public Object, virtual public IDisposable
{
public:
	using Super = Object;

private:
	TRefPtr<DirectXDynamicBufferAllocator> allocator;
	DirectXDynamicBufferAllocator::SingleBufferAllocatorHandle handle;

public:
	DirectXDynamicBuffer(DirectXDynamicBufferAllocator* inAllocator);
	~DirectXDynamicBuffer() override;

	virtual void Dispose();

	DirectXDynamicBufferAllocator* GetAllocator() const;
	uint64 GetGPUVirtualAddress() const;
	int8* GetBufferPointer() const;

protected:
	virtual void Dispose(bool bDisposing);
};