// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceResource.h"

#include "DirectXMinimal.h"

class DirectXDeviceContext;

class GAME_API DirectXCommandQueue : public DirectXDeviceResource
{
public:
	using Super = DirectXDeviceResource;

private:
	TComPtr<ID3D12CommandQueue> commandQueue;

public:
	DirectXCommandQueue(DirectXDeviceBundle* deviceBundle);
	DirectXCommandQueue(DirectXDeviceBundle* deviceBundle, D3D12_COMMAND_LIST_TYPE type);
	~DirectXCommandQueue() override;

	void ExecuteCommandLists(DirectXDeviceContext* const* deviceContexts, size_t count);

	vs_property_get_auto(ID3D12CommandQueue*, Item, commandQueue.Get());

public:
	inline void ExecuteCommandLists(DirectXDeviceContext* deviceContext);
	inline void ExecuteCommandLists(std::span<DirectXDeviceContext* const> deviceContexts);
};

#include "DirectXCommandQueue.inl"