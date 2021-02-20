// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include "D3D12Minimal.h"
#include "RHI/RHICommon.h"

struct RHIResourceDesc;

class D3D12DynamicBufferAllocator : virtual public Object
{
public:
	using Super = Object;
	using This = D3D12DynamicBufferAllocator;

private:
	const size_t capacity;
	const size_t nodeSize;
	const size_t fullSize;
	ComPtr<ID3D12Resource> buffer;
	void* addr;

	std::queue<size_t> myqueue;
	RHIResourceDesc bufferDesc;

public:
	D3D12DynamicBufferAllocator(size_t inCapacity, size_t inNodeSize);
	~D3D12DynamicBufferAllocator();

	RHIResourceDesc GetBufferDesc() const;
	uint64 GetVirtualAddress(size_t inNodeIdx) const;
	void* GetMappingAddress(size_t inNodeIdx) const;

	size_t Alloc();
	void Free(size_t inNodeIdx);
	
	vs_property_get(size_t, Slack);
	size_t Slack_get() const;
	vs_property_get_auto(size_t, NodeSize, nodeSize);
	vs_property_get_auto(ID3D12Resource*, Resource, buffer.Get());
};