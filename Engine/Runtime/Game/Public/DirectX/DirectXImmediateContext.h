// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeferredContext.h"

class DirectXCommandQueue;

class GAME_API DirectXImmediateContext : public DirectXDeferredContext
{
public:
	using Super = DirectXDeferredContext;

private:
	DirectXCommandQueue* commandQueue;

	std::vector<TRefPtr<Object>> pendingReferences;

public:
	DirectXImmediateContext(DirectXDeviceBundle* deviceBundle, DirectXCommandQueue* commandQueue, D3D12_COMMAND_LIST_TYPE type);
	~DirectXImmediateContext() override;

	void EndDraw() override;

	void AddPendingReference(Object* pendingReference);
	void AddPendingReference(IUnknown* pendingReference);
};