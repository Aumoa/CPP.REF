// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"

#include <mutex>

class D3D12DynamicBufferAllocator;
class D3D12DynamicBuffer;

class D3D12DynamicBufferManager : virtual public Object
{
public:
	using Super = Object;
	using This = D3D12DynamicBufferManager;

private:
	const size_t nodeSize;
	const size_t capacity;

	std::vector<TRefPtr<D3D12DynamicBufferAllocator>> allocators;
	std::mutex locker;

public:
	D3D12DynamicBufferManager(size_t inNodeSize);
	~D3D12DynamicBufferManager() override;

	TRefPtr<D3D12DynamicBuffer> AllocBuffer();
};