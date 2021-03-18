// Copyright 2020-2021 Aumoa.lib. All right reserved.

#include "DirectX/DirectXInstancedBufferAllocator.h"

#include "DirectXCommon.h"
#include "DirectX/DirectXDeviceBundle.h"

DirectXInstancedBufferAllocator::DirectXInstancedBufferAllocator(DirectXDeviceBundle* deviceBundle, size_t itemSize) : Super(deviceBundle)
	, ItemSize(itemSize)
	, bVolatile(true)

	, deviceBundle(deviceBundle)
	, count(0)
	, capacity(0)
{

}

DirectXInstancedBufferAllocator::~DirectXInstancedBufferAllocator()
{

}

void DirectXInstancedBufferAllocator::ShrinkToFit()
{
	SetCapacity(Count);
}

uint64 DirectXInstancedBufferAllocator::GetGPUVirtualAddress() const
{
	if (resource.IsValid)
	{
		return 0;
	}
	else
	{
		return resource->GetGPUVirtualAddress();
	}
}

int8& DirectXInstancedBufferAllocator::Get(size_t index)
{
	auto cast_ptr = (int8*)bufferPtr;
	return cast_ptr[ItemSize * index];
}

const int8& DirectXInstancedBufferAllocator::Get(size_t index) const
{
	auto cast_ptr = (int8*)bufferPtr;
	return cast_ptr[ItemSize * index];
}

size_t DirectXInstancedBufferAllocator::Count_get() const
{
	return count;
}

void DirectXInstancedBufferAllocator::Count_set(size_t value)
{
	if (capacity >= value)
	{
		count = value;
		return;
	}

	SetCapacity(value);
	count = value;
}

void DirectXInstancedBufferAllocator::SetCapacity(size_t newCapacity)
{
	if (!bVolatile)
	{
		throw NotSupportedException(L"Instanced buffer without volatile attribute is not supported yet.");
	}

	if (newCapacity == 0)
	{
		resource.Reset();
	}
	else
	{
		resource = deviceBundle->CreateDynamicBuffer(ItemSize * newCapacity);
		HR(resource->Map(0, nullptr, &bufferPtr));
	}

	capacity = newCapacity;
}