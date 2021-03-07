// Copyright 2020-2021 Aumoa.lib. All right reserved.

#pragma once

#include "GameAPI.h"
#include "CoreMinimal.h"
#include "DirectXDeviceContext.h"

class DirectXCommandQueue;

class GAME_API DirectXImmediateContext : public DirectXDeviceContext
{
public:
	using Super = DirectXDeviceContext;

private:
	TRefPtr<DirectXCommandQueue> commandQueue;

public:
	DirectXImmediateContext(DirectXDeviceBundle* deviceBundle, DirectXCommandQueue* commandQueue, D3D12_COMMAND_LIST_TYPE type);
	~DirectXImmediateContext() override;
};