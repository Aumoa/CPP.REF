// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

#include "DirectXMinimal.h"

class GAME_API DirectXInstancedBufferAllocator : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	DirectXDeviceBundle* deviceBundle;
	TComPtr<ID3D12Resource> resource;
	void* bufferPtr;
	size_t count;
	size_t capacity;

public:
	DirectXInstancedBufferAllocator(DirectXDeviceBundle* deviceBundle, size_t itemSize);
	~DirectXInstancedBufferAllocator() override;

	void ShrinkToFit();
	uint64 GetGPUVirtualAddress() const;

	int8& Get(size_t index);
	const int8& Get(size_t index) const;
	template<class T, class... TConstructorArgs>
	T& Emplace(size_t index, TConstructorArgs&&... inArgs);

	vs_property(size_t, Count);
	vs_property_get_auto(size_t, Capacity, capacity);
	vs_property_get_auto(ID3D12Resource*, Resource, resource.Get());
	const size_t ItemSize;
	const bool bVolatile : 1;

private:
	void SetCapacity(size_t newCapacity);
};

#include "DirectXInstancedBufferAllocator.inl"