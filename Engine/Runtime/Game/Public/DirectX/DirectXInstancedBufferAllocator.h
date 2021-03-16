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
	const bool bDynamic : 1;
	TComPtr<ID3D12Resource> resource;
	size_t count;
	size_t capacity;

public:
	DirectXInstancedBufferAllocator(DirectXDeviceBundle* deviceBundle, bool bDynamic = true);
	~DirectXInstancedBufferAllocator() override;

	vs_property(size_t, Count);
	vs_property_get_auto(size_t, Capacity, capacity);
};