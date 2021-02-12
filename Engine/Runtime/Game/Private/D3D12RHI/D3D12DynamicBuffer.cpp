// Copyright 2020 Aumoa.lib. All right reserved.

#include "D3D12DynamicBuffer.h"

#include "D3D12DynamicBufferAllocator.h"

D3D12DynamicBuffer::D3D12DynamicBuffer(size_t index, D3D12DynamicBufferAllocator* inAllocator) : Super()
	, idx(index)
	, allocator(inAllocator)
{

}

D3D12DynamicBuffer::~D3D12DynamicBuffer()
{

}

uint64 D3D12DynamicBuffer::GetVirtualAddress() const
{
	return allocator->GetVirtualAddress(idx);
}

void* D3D12DynamicBuffer::GetMappingAddress() const
{
	return allocator->GetMappingAddress(idx);
}

RHIResourceDesc D3D12DynamicBuffer::GetDesc() const
{
	auto desc = allocator->GetBufferDesc();
	desc.Width = allocator->NodeSize;
	return desc;
}

ID3D12Resource* D3D12DynamicBuffer::Resource_get() const
{
	return allocator->Resource;
}