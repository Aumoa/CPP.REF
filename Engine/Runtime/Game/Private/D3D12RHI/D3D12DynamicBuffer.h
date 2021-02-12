// Copyright 2020 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "RHI/IRHIResource.h"
#include "ID3D12ResourceBase.h"

#include "D3D12Minimal.h"

class D3D12DynamicBufferAllocator;

class D3D12DynamicBuffer : virtual public Object, virtual public IRHIResource, virtual public ID3D12ResourceBase
{
public:
	using Super = Object;
	using This = D3D12DynamicBuffer;

private:
	const size_t idx;
	D3D12DynamicBufferAllocator* const allocator;

public:
	D3D12DynamicBuffer(size_t index, D3D12DynamicBufferAllocator* inAllocator);
	~D3D12DynamicBuffer() override;

	virtual uint64 GetVirtualAddress() const;
	virtual void* GetMappingAddress() const;
	virtual RHIResourceDesc GetDesc() const;

	virtual ID3D12Resource* Resource_get() const;
};