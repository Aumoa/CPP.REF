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
	struct PendingReference
	{
		uint64 FenceValue;
		TRefPtr<Object> ObjectReference;
	};

private:
	TComPtr<ID3D12CommandQueue> commandQueue;
	TComPtr<ID3D12Fence> fence;
	uint64 fenceValue;
	std::queue<PendingReference> pendingReferences;

public:
	DirectXCommandQueue(DirectXDeviceBundle* deviceBundle);
	DirectXCommandQueue(DirectXDeviceBundle* deviceBundle, D3D12_COMMAND_LIST_TYPE type);
	~DirectXCommandQueue() override;

	void ExecuteCommandLists(DirectXDeviceContext* const* deviceContexts, size_t count);
	void AddPendingReference(Object* inPendingReference);
	void CollectPendingReferences();

	vs_property_get_auto(ID3D12CommandQueue*, Item, commandQueue.Get());

public:
	inline void ExecuteCommandLists(DirectXDeviceContext* deviceContext);
	inline void ExecuteCommandLists(std::span<DirectXDeviceContext* const> deviceContexts);
};

#include "DirectXCommandQueue.inl"